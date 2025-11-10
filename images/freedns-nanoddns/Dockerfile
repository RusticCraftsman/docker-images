# freedns-nanoddns

# Set up build system and deps
FROM alpine:3.20 AS builder
RUN apk add --no-cache gcc musl-dev

# Compile
COPY main.c /src/
RUN gcc -Ofast -static -s /src/main.c -o /target

# Set up runtime env
FROM scratch
COPY --from=builder /target /target
VOLUME ["/data"]
ENTRYPOINT ["/target"]


LABEL maintainer="https://github.com/RusticCraftsman/docker-images"
LABEL description="<100kb HTTP(insecure) freedns ddns service"

LABEL version="${VERSION}"
LABEL build_date="${BUILD_DATE}"
LABEL commit_sha="${COMMIT_SHA}"
LABEL org.opencontainers.image.version="${VERSION}"
