#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "greet.grpc.pb.h"

#define UNUSED(x) (void)(x);

class ServiceImplementation final : public ::greet::Greeter::Service
{
	::grpc::Status SayHello(::grpc::ServerContext *ctx, const ::greet::HelloRequest* request, ::greet::HelloReply* reply) override {
		UNUSED(request)
		UNUSED(reply)
		UNUSED(ctx)
		return ::grpc::Status::OK;
	}
};

int main(int argc, char* argv[])
{
	UNUSED(argc)
	UNUSED(argv)

	std::string address("0.0.0.0:5555");
	ServiceImplementation service;

	::grpc::ServerBuilder builder;
	builder.AddListeningPort(address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
	std::cout << "Started serer on " << address << std::endl;

	server->Wait();

	return 0;
}
