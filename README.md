Miscellaneous experiments

If not noted otherwise in the sources, it is [CLL](http://permalink.de/tino/cll), see [COPYRIGHT.CLL](COPYRIGHT.CLL).

Recommendations:
----------------

- force_local/ is a recipe to use LD_PRELOAD to patch programs to listen or bind to fixed ports, such that you can apply static firewall rules to them.

- wrap/ is a wrapper to alter the environment of SUID programms and installs to /wrap/ - it can be used to  apply force_local/ fixes to SUID programs.

- src/ipof is a easy to use, highly configurable, fast and extremly small IP resolver for batch and scripting purpose and works for IPv4 and IPv6.  It's source is less than 8K.

- porthog/ allocates IP port ranges and keeps them open while another command runs.  This prevents the running program from binding to that ports.
