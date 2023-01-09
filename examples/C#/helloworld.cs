using System;
using Grpc.Core;	
using Aurum;
		
namespace GrpcSample
{
    static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Environment.SetEnvironmentVariable("GRPC_CSHARP_EXT_OVERRIDE_LOCATION", "/usr/lib/libgrpc_csharp_ext.so", EnvironmentVariableTarget.Process);
            Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            var client = new Bootstrap.BootstrapClient(channel);
            var response = client.getDeviceTime(new ReqGetDeviceTime{ Type = ReqGetDeviceTime.Types.TimeType.Wallclock });
            Console.WriteLine(response.LocaleDatetime);	
        }
    }	
}
