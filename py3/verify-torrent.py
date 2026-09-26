#!/usr/bin/env python3
"""
verify_torrent.py – offline verification of a torrent against local data.

Usage:
    python3 verify_torrent.py <data_directory> <torrent_file..>

Exit code 0 = all pieces valid, 1 = verification failed.

This was written by some AI and only a slightly bit improved manually.
It still opens some UDP port on localhost and does some NETLINK queries.
I have no idea currently how to suppress this.
"""

import	sys
import	time
import	libtorrent as lt

def verify(data_dir: str, torrent_file: str) -> bool:
	# Session with no network activity
	# (however, trackers are still honored below)
	ses	= lt.session(	{"listen_interfaces":	"127.0.0.1:0"
				,"download_rate_limit":	0
				,"upload_rate_limit":	0
				,"enable_outgoing_utp":	False
				,"enable_incoming_utp":	False
				,"enable_outgoing_tcp":	False
				,"enable_incoming_tcp":	False
				,"enable_upnp":		False
				,"enable_natpmp":	False
				,"enable_lsd":		False
				,"enable_dht":		False
				,"anonymous_mode":	False
				})

	atp		= lt.add_torrent_params()
	atp.ti		= lt.torrent_info(torrent_file)
	atp.save_path	= data_dir
	atp.seed_mode	= True   # "I believe I have all data; just verify"

	handle		= ses.add_torrent(atp)
	handle.replace_trackers({})		# do not connect to trackers

	# Force a full recheck of every piece
	handle.force_recheck()

	print(f"Save path: {data_dir}")
	print(f"Verifying: {atp.ti.name()}")

	while True:
		s = handle.status()
		if s.state == lt.torrent_status.checking_files:
			print(f"\rChecking… {s.progress * 100:6.2f}%", end="", flush=True)
			time.sleep(0.5)
		elif s.state == lt.torrent_status.seeding:
			print("\rVerified:  ✓ All pieces verified successfully.")
			ses.remove_torrent(handle)
			return True
		elif s.state == lt.torrent_status.downloading:
			print("\rFailed:    ✗ Verification FAILED – some pieces are missing or corrupted.")
			ses.remove_torrent(handle)
			return False
		else:
			time.sleep(0.5)

if __name__ == "__main__":
	if len(sys.argv) < 3:
		print(f"Usage: {sys.argv[0]} <data_directory> <torrent_file..>")
		sys.exit(2)
	for torrent_file in sys.argv[2:]:
		if not verify(sys.argv[1], torrent_file):
			sys.exit(1)
	sys.exit(0)
 
