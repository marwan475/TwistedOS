#!/bin/bash

# Create the bridge interface
sudo ip link add br0 type bridge

# Create the TAP interface
sudo ip tuntap add dev tap0 mode tap

# Add the TAP and physical interface to the bridge
sudo ip link set dev tap0 master br0
sudo ip link set dev eth0 master br0

# Bring up the bridge and TAP interfaces
sudo ip link set dev br0 up
sudo ip link set dev tap0 up

# Remove IP address from eth0
sudo ip address delete 192.168.1.100/24 dev eth0

# Assign IP address to the bridge
sudo ip address add 192.168.1.100/24 dev br0

# Add default route
sudo ip route add default via 192.168.1.1 dev br0

# Show the network interfaces
ip addr show

