# KookminUniv

#install git

$sudo apt-get install git git-core
$git clone https://github.com/hgk333/KookminUniv.git

then you can see the folder naming KookminUniv

you should copy every thing to /home/pi from KookminUniv.git folder

the command is $cp ***.*** ~/ 

#input your email address and passwd at e_mail.py

"***@gmail.com"
"password"


#auto send
sudo vi /etc/rc.local
input the 

sudo python /home/pi/e_mail.py &

sleep 10

sudo python /home/pi/e_mail.py &

before exit 0


#vimrc 
this is setting file for vim
at KookminUniv folder

$cp vimrc ~/.vimrc

that's all. 
