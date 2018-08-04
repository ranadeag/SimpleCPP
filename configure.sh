#!/bin/bash

declare -a packages=(libglfw3-dev libglfw3 libglew-dev libglc-dev libsoil-dev libglm-dev)

for i in "${packages[@]}"
do
   dpkg -s "$i" &> /dev/null

	if [ $? -ne 0 ]; then
		sudo apt-get install $i
	fi
done

unamestr=$(uname)
if [ "$unamestr" = 'Linux' ]; then
	MAKEOPT="-I"`pwd`"/include -std=c++17"
elif [ "$unamestr" = 'Darwin' ]; then
        MAKEOPT="-I"`pwd`"/include -std=c++17"
fi

rm -f makes++
echo "#!/bin/sh -x" >> makes++
echo "g++ \$@ -Wall "$MAKEOPT >> makes++
chmod +x makes++



cd src
make config
cd ..

rm -f s++

SPRITE=`pwd`"/lib/libsprite.a "$MAKEOPT

echo "#!/bin/sh -x" >> s++
echo "g++ \$@ -Wall "$SPRITE " -lGL -lGLEW -lglfw -lSOIL -lGLC">> s++

chmod +x s++
rm src/*.o

sudo cp s++ /usr/bin
echo "Cammand For Compilation:  s++ filename.cpp"

