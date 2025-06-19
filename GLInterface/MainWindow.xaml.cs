using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.IO;
using System.IO.Pipes;
using System.Diagnostics;
using System.Net.Http;
using Microsoft.Win32;
using System.Management;
using System.Net.Sockets;

namespace WpfApp1
{

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        static bool Injected = false;

        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!Injected)
            {
                MessageBox.Show("Not injected!", "GlExecutor", MessageBoxButton.OK, MessageBoxImage.Hand);
            }

            TextRange tx = new TextRange(richbich.Document.ContentStart, richbich.Document.ContentEnd);

            string url = "http://localhost:7646/GLExecutor_ExecuteRequest";

            try
            {
                using (HttpClient client = new HttpClient())
                {
                    var content = new StringContent(tx.Text, Encoding.UTF8, "text/plain");
                    await client.PostAsync(url, content);
                }

            }
            catch (Exception ex)
            {

                MessageBox.Show("There was an error executing the script: " + ex.Message, "GlExecutor", MessageBoxButton.OK, MessageBoxImage.Hand);
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            ProcessStartInfo proc = new ProcessStartInfo
            {
                 FileName = "GLExternal.exe",
                 Verb = "runas",
                 UseShellExecute = false,
            };
            Process.Start(proc);

            Injected = true;
        }
    }
}
