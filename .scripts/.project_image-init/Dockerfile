# {{PROJECT_NAME}}

# FROM ubuntu:24.04
# FROM alpine:3.20
# WORKDIR /src
# EXPOSE 0
# CMD ["python", "main.py"]

# FROM scratch
# COPY --from=builder /target /target
VOLUME ["/data"]
# ENTRYPOINT ["/target"]

LABEL maintainer="https://github.com/RusticCraftsman/docker-images"
# LABEL description="A basic description."

LABEL version="${VERSION}"
LABEL build_date="${BUILD_DATE}"
LABEL commit_sha="${COMMIT_SHA}"
LABEL org.opencontainers.image.version="${VERSION}"
