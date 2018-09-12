using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Interfaccia
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public void ChooseFolder()
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ChooseFolder();
        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void maskedTextBox1_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            System.IO.File.WriteAllText("../../../Files/path.txt", textBox1.Text);
            System.IO.File.WriteAllText("../../../Files/righeA.txt", maskedTextBox1.Text);
            System.IO.File.WriteAllText("../../../Files/colonneA.txt", maskedTextBox2.Text);
            System.IO.File.WriteAllText("../../../Files/colonneB.txt", maskedTextBox3.Text);
            //altre textbox
            Process.Start(@"C:/Users/Frenk/Documents/Visual Studio 2017/Projects/GraphicProjectSvasi/x64/Debug/GraphicProjectSvasi.exe");
        }

        private void tabPage3_Click(object sender, EventArgs e)
        {

        }
    }
}
