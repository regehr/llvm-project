Hyperthreading off in BIOS
    
```
echo 0 > /proc/sys/kernel/randomize_va_space

for i in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
do
  echo performance > $i
done

systemctl set-property --runtime -- user.slice AllowedCPUs=0-7
systemctl set-property --runtime -- system.slice AllowedCPUs=0-7
systemctl set-property --runtime -- init.scope AllowedCPUs=0-7

cpupower frequency-info | grep 'boost state support' -A2 | grep Active

taskset -c 8 ./go.sh
```
```
python3 /home/regehr/libyuv/util/run_libyuv_benchmarks.py \
    --exe /home/regehr/libyuv/build/libyuv_unittest \
    --output /tmp/libyuv_width_opt.csv \
    --width 1280 \
    --height 720 \
    --repeat 1000 \
    --trials 3
```