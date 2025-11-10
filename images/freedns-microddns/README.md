# freedns-microddns
> neverblue/freedns-microddns

A very simple, lightweight and minimal dockerized **HTTPS** DDNS server [freedns](https://freedns.afraid.org/)

* Written in C
* Less than 1MB
* Probably unsafe
* TLS thanks to mbedtls

## Docker compose

``` yml
services:
  freedns-microddns:
    build: .
    container_name: freedns-microddns
    restart: unless-stopped
    environment:
      # Your FreeDNS update hash from https://freedns.afraid.org/dynamic/
      FREEDNS_HASH: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
      # Update interval in second
      INTERVAL: "300"
    volumes:
      - ./data:/data
```

## Source :
https://github.com/RusticCraftsman/docker-images






