# {{PROJECT_NAME}}
> neverblue/{{PROJECT_NAME}}

Simple description

## Docker compose

``` yml
services:
  {{PROJECT_NAME}}:
      image: neverblue/{{PROJECT_NAME}}:latest
    restart: unless-stopped
    ports:
      # - "0:0"
    environment:
      # - VARIABLE = true
    volumes:
      - ./data:/data
```

## Source :
https://github.com/RusticCraftsman/docker-images






