# Bash commands to look at motion captured video over network

sudo mount 192.168.50.1:/mnt/My_Passport/capture /nfs/general

rm -Rf /home/plantz/Documents/videos/*

cd /nfs/general

cp 2020-04-12_11* /home/plantz/Documents/videos
 
vlc /home/plantz/Documents/videos/
