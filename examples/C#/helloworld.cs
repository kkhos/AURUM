using System;
using Tizen.NUI;
using Tizen.NUI.BaseComponents;

using Grpc.Core;
using Aurum;

namespace GrpcSample
{
    static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            Tizen.NUI.Accessibility.Accessibility.BridgeDisableAutoInit();
            Environment.SetEnvironmentVariable("GRPC_CSHARP_EXT_OVERRIDE_LOCATION", "/usr/lib/libgrpc_csharp_ext.so.20", EnvironmentVariableTarget.Process);

            Scene1 Instance = new Scene1();
            Instance.Run(args);
        }
    }

    public partial class Scene1 : NUIApplication
    {
        override protected void OnCreate()
        {
            base.OnCreate();
            Channel channel = new Channel("127.0.0.1:50051", ChannelCredentials.Insecure);
            var client = new Bootstrap.BootstrapClient(channel);
            var response = client.getDeviceTime(new ReqGetDeviceTime{ Type = ReqGetDeviceTime.Types.TimeType.Wallclock });
            Console.WriteLine(response.LocaleDatetime);
        }
    }
}
