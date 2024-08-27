#!/bin/bash

# runner.sh
# Aarav Rekhi
# aaravr
# ECE 2230 Spring 2024
# MP3

# Script to quickly run many sorting expirements


# set these lists with the parameter space's values
sizes='100 500 1000 10000 50000'

# descending,  ascending, random
orders='1 2 3'

# sorting algorithm IDs
sorts='1 2 3 4 5'



for s in $sorts
do
    echo "Method: $s"

    for o in $orders
    do
        echo "Data Order: $o"
        
        for n in $sizes
        do
            echo $n $o $s
            ./geninput $n $o $s 1 | ./lab3 1
            ./geninput $n $o $s 1 | ./lab3 1
            ./geninput $n $o $s 1 | ./lab3 1
            ./geninput $n $o $s 1 | ./lab3 1
            ./geninput $n $o $s 1 | ./lab3 1
        done
    done
done