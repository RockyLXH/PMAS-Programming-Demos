#!/bin/sh

# description: controls SIL runtime activation
SCRIPT=$(readlink -f "$0")
#echo SCRIPT = $SCRIPT
SCRIPTPATH=`dirname "$SCRIPT"`
#echo SCRIPTPATH =  $SCRIPTPATH
#export LD_LIBRARY_PATH=$SCRIPTPATH/lib

pid=`ps | grep "SIL_BASE.\+exe" | grep -v grep | awk '{print $1}'`
ext=`uname -a | cut -d" " -f2`
[ "PMAS" == "$ext" ] && bin="pexe" || bin="gexe"
echo "bin=$bin"
[ -z "$1" ] && _op='status' || _op="$1" 
[ $# -ge 1 ] && shift
echo "_op=$_op, args=$@"
case "$_op" in
  start)
		cd /mnt/jffs/usr
		[ -z "$pid" ] && /mnt/jffs/usr/SIL_BASE.${bin} $@ &
		cd -
        	;;
  stop)
		[ ! -z "$pid" ] && kill -10 ${pid}
        	;;
		
  startup)
		rm -rf /mnt/jffs/usr/UserStartUp.sh
		touch /mnt/jffs/usr/UserStartUp.sh
		chown user:1001 /mnt/jffs/usr/UserStartUp.sh
		chmod 777 /mnt/jffs/usr/UserStartUp.sh
		echo "export LD_LIBRARY_PATH=${SCRIPTPATH}/lib" > ${SCRIPTPATH}/UserStartUp.sh
		echo "cd /mnt/jffs/usr && /mnt/jffs/usr/SIL_BASE.$bin} ${@} &" >> /mnt/jffs/usr/UserStartUp.sh
		;;
  nostartup)
		rm -rf /mnt/jffs/usr/UserStartUp.sh
		;;
  status)
		echo -ne "\e[33mstatus:\e[0m"'\t'	
		[ -z "$pid" ] && echo -e '\e[31moff\e[0m' || echo -e '\e[32mon\e[0m'
       		;;
  *)
		;;
esac

pid=`ps | grep "SIL_BASE.\+exe" | grep -v grep | awk '{print $1}'`
[ -z "$pid" ] && exit 0 || exit 1
