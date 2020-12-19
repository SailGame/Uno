/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using SailGame::IntWrapper;
using SailGame::Hello;

class HelloClient {
 public:
  HelloClient(std::shared_ptr<Channel> channel)
      : stub_(Hello::NewStub(channel)) {}

  void RouteChat() {
    ClientContext context;

    std::shared_ptr<ClientReaderWriter<IntWrapper, IntWrapper> > stream(
        stub_->BiStream(&context));

    stream->WritesDone();

    Status status = stream->Finish();
    if (!status.ok()) {
      std::cout << "RouteChat rpc failed." << std::endl;
    }
  }

 private:
    std::unique_ptr<Hello::Stub> stub_;
};

int main(int argc, char** argv) {
  HelloClient client(grpc::CreateChannel("localhost:50051", 
    grpc::InsecureChannelCredentials()));

  client.RouteChat();

  return 0;
}
