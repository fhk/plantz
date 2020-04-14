[root@DAFANG:motiondetection]
# cat nasVideo.sh
#!/bin/sh
# progname : nasVideo
# delivers NUMSECS second clip to nas share
# backgrounded to allow immediate return
# creates nfs mount on call if not already mounted ( does not record in that pass)

NUMSECS=10
NASMOUNT=/system/media/nas


if grep -qs "${NASMOUNT} "  /proc/mounts; then
    FILE=`date +%Y-%m-%d_%H.%M.%S.camera1.h264`
    timeout -t 11 /system/sdcard/bin/avconv -rtsp_transport tcp -y -i rtsp://0.0.0.0:8554/unicast -vcodec copy -an ${NASMOUNT}/${FILE} -t 10 &>/dev/null &
    LAST_PID=$!
    sleep 10
    kill -9 ${LAST_PID}
else
    echo " no mount point: ${NASMOUNT} "
    mkdir -p ${NASMOUNT}
    mount -o nolock -t nfs 192.168.50.1:/mnt/My_Passport/capture ${NASMOUNT}
fi
