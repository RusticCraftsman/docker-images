# freedns-nanoddns
> neverblue/freedns-nanoddns

A very simple, extremely lightweight and minimal dockerized **HTTP** DDNS server designed for [freedns](https://freedns.afraid.org/)

* Written in C
* Less than 100KB (67KB last i checked)
* Probably unsafe

## Docker compose

``` yml
services:
  freedns-nanoddns:
      image: neverblue/freedns-nanoddns:latest
    restart: unless-stopped
    environment:
      # Your FreeDNS update hash from https://freedns.afraid.org/dynamic/
      FREEDNS_HASH: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
      # Update interval in seconds
      INTERVAL: "300"
    volumes:
      - ./data:/data
```

## Source :
https://github.com/RusticCraftsman/docker-images
