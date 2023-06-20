#!/bin/sh
gnome-terminal -- ./srvr 1234 .
gnome-terminal -- ./clnt 127.0.1.1 1234
