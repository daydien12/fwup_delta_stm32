if [ -e /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.Apptest_stm32/MDK/Objects/gpio_blynk.bin ]
then
    mv  /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.Apptest_stm32/MDK/Objects/gpio_blynk.bin .
    ls -alh
    sleep 1
    rm -r old.bin
    sleep 1
    mv new.bin old.bin
    sleep 1
    rm -r patch.patch
    sleep 1
    mv gpio_blynk.bin new.bin
    sleep 1
    ./jdiff.exe old.bin new.bin patch.patch
else
    echo "Không tìm thấy file gpio_blynk.bin"
fi