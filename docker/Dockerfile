### Build/test container ###
# Define builder stage
FROM sffs:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
# Define deploy stage
FROM ubuntu:focal as deploy

# RUN mkdir -p ./bin
COPY --from=builder /usr/src/project/build/bin/server .
COPY --from=builder /usr/src/project/cloud_config .
COPY --from=builder /usr/src/project/server_files ./tests/server_files
COPY --from=builder /usr/src/project/server_files ./tests/server_files2
COPY --from=builder /usr/src/project/server_files ./tests/test_files/md
# Expose some port(s)
# NOTE: if we change ports, this needs to be updated
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
# WORKDIR "./bin"
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
CMD ["cloud_config"]
