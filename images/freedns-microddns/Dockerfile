# freedns-microddns

FROM alpine:3.20 AS builder
RUN apk add --no-cache gcc musl-dev mbedtls-dev mbedtls-static

COPY main.c /src/
RUN gcc -Os -static -s -ffunction-sections -fdata-sections \
    -Wl,--gc-sections -Wl,--strip-all \
    /src/main.c \
    -lmbedtls -lmbedx509 -lmbedcrypto \
    -o /target && \
    strip --strip-unneeded /target

FROM scratch
COPY --from=builder /target /target
VOLUME ["/data"]
ENTRYPOINT ["/target"]

LABEL maintainer="https://github.com/RusticCraftsman/docker-images"
LABEL description="<1MB TLS freedns ddns service"

LABEL version="${VERSION}"
LABEL build_date="${BUILD_DATE}"
LABEL commit_sha="${COMMIT_SHA}"
LABEL org.opencontainers.image.version="${VERSION}"
