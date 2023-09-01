echo "Enable port"

sudo chmod 777 /dev/ttyUSB0
sudo chmod 777 /dev/ttyUSB1

echo "Build"

make

echo "run"
echo ""

./uart_mqtt