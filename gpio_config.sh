#!/bin/sh
#Define some variables
ERROR_MESSAGE="use [script_name] [Arg1: Pin Number] [Arg2: in or out] "
case $# in
	1)
			  echo $1 > /sys/class/gpio/unexport
			  break
			  ;;
	2)
			  echo $1 > /sys/class/gpio/export
			  sleep 1
			  echo $2 > /sys/class/gpio/gpio$1/direction
			  break
			  ;;
	*)
			  echo $ERROR_MESSAGE
			  break
			  ;;
esac
