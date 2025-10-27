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
