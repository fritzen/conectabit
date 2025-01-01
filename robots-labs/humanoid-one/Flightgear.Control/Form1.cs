using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO.Ports;
using System.IO;

namespace Flightgear.Control
{
    public partial class Form1 : Form
    {

        BindingList<CsvData> listPositions = new BindingList<CsvData>();
        const string sPath = "save.txt";

        public Form1()
        {
            InitializeComponent();
        }

        //Receive from Serial port
        private SerialPort serial_port_ = null;

        //Threads
        private Thread thread_receive_from_serial_port_     = null;

        
        //Refresh button
        private void refresh_button_Click(object sender, EventArgs e)
        {
            portlist_comboBox.Items.Clear();
            foreach (string s in SerialPort.GetPortNames())
            {
                portlist_comboBox.Items.Add(s);
            }
        }
        
        //Connect buttons
        private void connect_serial_button_Click(object sender, EventArgs e)
        {
            if (null == thread_receive_from_serial_port_)
            {
                if (-1 == portlist_comboBox.SelectedIndex)
                {
                    MessageBox.Show("Select Com port");
                    return;
                }
                string port_name = portlist_comboBox.SelectedItem.ToString(); // get serial port name
                serial_port_ = new SerialPort(port_name, 9600); // set port name and speed 9600 bits/second
                serial_port_.Open(); // connect to port

                thread_receive_from_serial_port_ = new Thread(receive_from_serial_port);
                thread_receive_from_serial_port_.Start();
                connect_serial_button.Text = "Disconnect";
                
            }
            else
            {
                thread_receive_from_serial_port_.Abort();
                thread_receive_from_serial_port_ = null;
                connect_serial_button.Text = "Connect";

                serial_port_.Close();
                serial_port_ = null;

            }
        }


        delegate void SetTextCallback(string text);

        private void SetText(string text)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.textBoxOutput.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.textBoxOutput.Text = this.textBoxOutput.Text + text;
            }
        }


        private void receive_from_serial_port()
        {
            while (serial_port_.IsOpen)
            {
                try
                {
                    string str = serial_port_.ReadLine(); //
                    if (!String.IsNullOrEmpty(str)) {
                        SetText(str + "\r\n");
                    }
                    
                }
                catch (TimeoutException) { }
                Thread.Sleep(100);
            }
        }

    
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.SERVO_MAO_ESQUERDA = trackBar_mao_esquerda.Value;

            Properties.Settings.Default.SERVO_BRACO_ESQUERDO = trackBar_cotovelo_esquerdo.Value;

            Properties.Settings.Default.SERVO_OMBRO_ESQUERDO = trackBar_ombro_esquerdo.Value;

            Properties.Settings.Default.SERVO_MAO_DIREITA = trackBar_mao_direita.Value;

            Properties.Settings.Default.SERVO_BRACO_DIREITO = trackBar_cotovelo_direito.Value;

            Properties.Settings.Default.SERVO_OMBRO_DIREITO = trackBar_ombro_direito.Value;

            Properties.Settings.Default.SERVO_QUADRIL_ESQUERDO = trackBar_quadril_esquerdo.Value;

            Properties.Settings.Default.SERVO_JOELHO_1_ESQUERDO = trackBar_joelho_esquerdo_1.Value;

            Properties.Settings.Default.SERVO_JOELHO_2_ESQUERDO = trackBar_joelho_esquerdo_2.Value;

            Properties.Settings.Default.SERVO_TORNOZELO_ESQUERDO = trackBar_tornozelo_esquerdo.Value;

            Properties.Settings.Default.SERVO_PE_ESQUERDO = trackBar_pe_esquerdo.Value;

            Properties.Settings.Default.SERVO_QUADRIL_DIREITO = trackBar_quadril_direito.Value;

            Properties.Settings.Default.SERVO_JOELHO_1_DIREITO = trackBar_joelho_direito_1.Value;

            Properties.Settings.Default.SERVO_JOELHO_2_DIREITO = trackBar_joelho_direito_2.Value;

            Properties.Settings.Default.SERVO_TORNOZELO_DIREITO = trackBar_tornozelo_direito.Value;

            Properties.Settings.Default.SERVO_PE_DIREITO = trackBar_pe_direito.Value;


            Properties.Settings.Default.Save();
            if (null != thread_receive_from_serial_port_)
            {
                thread_receive_from_serial_port_.Abort();
                thread_receive_from_serial_port_ = null;

                serial_port_.Close();
                serial_port_ = null;
            }

        }

        private void SendSerialData(String data) {
            if (serial_port_ != null)
            {
                serial_port_.Write("<" + data + ">");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            listBox1.DataSource = listPositions;
            
            listBox1.DisplayMember = "Name";

            trackBar_mao_esquerda.Value = Properties.Settings.Default.SERVO_MAO_ESQUERDA;
            lblMaoEsquerda.Text = trackBar_mao_esquerda.Value.ToString();

            trackBar_cotovelo_esquerdo.Value = Properties.Settings.Default.SERVO_BRACO_ESQUERDO;
            lblCotoveloEsquerdo.Text = trackBar_cotovelo_esquerdo.Value.ToString();

            trackBar_ombro_esquerdo.Value = Properties.Settings.Default.SERVO_OMBRO_ESQUERDO;
            lblOmbroEsquerdo.Text = trackBar_ombro_esquerdo.Value.ToString();


            trackBar_mao_direita.Value = Properties.Settings.Default.SERVO_MAO_DIREITA;
            lblMaoDireita.Text = trackBar_mao_direita.Value.ToString();

            trackBar_cotovelo_direito.Value = Properties.Settings.Default.SERVO_BRACO_DIREITO;
            lblCotoveloDireito.Text = trackBar_cotovelo_direito.Value.ToString();

            trackBar_ombro_direito.Value = Properties.Settings.Default.SERVO_OMBRO_DIREITO;
            lblOmbroDireito.Text = trackBar_ombro_direito.Value.ToString();

            trackBar_quadril_esquerdo.Value = Properties.Settings.Default.SERVO_QUADRIL_ESQUERDO;
            lblQuadrilEsquerdo.Text = trackBar_quadril_esquerdo.Value.ToString();

            trackBar_joelho_esquerdo_1.Value = Properties.Settings.Default.SERVO_JOELHO_1_ESQUERDO;
            lblJoelhoEsquerdo01.Text = trackBar_joelho_esquerdo_1.Value.ToString();

            trackBar_joelho_esquerdo_2.Value = Properties.Settings.Default.SERVO_JOELHO_2_ESQUERDO;
            lblJoelhoEsquerdo02.Text = trackBar_joelho_esquerdo_2.Value.ToString();

            trackBar_tornozelo_esquerdo.Value = Properties.Settings.Default.SERVO_TORNOZELO_ESQUERDO;
            lblTornozeloEsquerdo.Text = trackBar_tornozelo_esquerdo.Value.ToString();

            trackBar_pe_esquerdo.Value = Properties.Settings.Default.SERVO_PE_ESQUERDO;
            lblPeEsquerdo.Text = trackBar_pe_esquerdo.Value.ToString();

            trackBar_quadril_direito.Value = Properties.Settings.Default.SERVO_QUADRIL_DIREITO;
            lblQuadrilDireito.Text = trackBar_quadril_direito.Value.ToString();

            trackBar_joelho_direito_1.Value = Properties.Settings.Default.SERVO_JOELHO_1_DIREITO;
            lblJoelhoDireito1.Text = trackBar_joelho_direito_1.Value.ToString();

            trackBar_joelho_direito_2.Value = Properties.Settings.Default.SERVO_JOELHO_2_DIREITO;
            lblJoelhoDireito2.Text = trackBar_joelho_direito_2.Value.ToString();

            trackBar_tornozelo_direito.Value = Properties.Settings.Default.SERVO_TORNOZELO_DIREITO;
            lblTornozeloDireito.Text = trackBar_tornozelo_direito.Value.ToString();

            trackBar_pe_direito.Value = Properties.Settings.Default.SERVO_PE_DIREITO;
            lblPeDireito.Text = trackBar_pe_direito.Value.ToString();

          

        }

        private void trackBar_mao_esquerda_Scroll(object sender, EventArgs e)
        {
            lblMaoEsquerda.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_cotovelo_esquerdo_Scroll(object sender, EventArgs e)
        {
            lblCotoveloEsquerdo.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_ombro_esquerdo_Scroll(object sender, EventArgs e)
        {
            lblOmbroEsquerdo.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_quadril_esquerdo_Scroll(object sender, EventArgs e)
        {
            lblQuadrilEsquerdo.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_joelho_esquerdo_1_Scroll(object sender, EventArgs e)
        {
            lblJoelhoEsquerdo01.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_joelho_esquerdo_2_Scroll(object sender, EventArgs e)
        {
            lblJoelhoEsquerdo02.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_tornozelo_esquerdo_Scroll(object sender, EventArgs e)
        {
            lblTornozeloEsquerdo.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_pe_esquerdo_Scroll(object sender, EventArgs e)
        {
            lblPeEsquerdo.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_mao_direita_Scroll(object sender, EventArgs e)
        {
            lblMaoDireita.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_cotovelo_direito_Scroll(object sender, EventArgs e)
        {
            lblCotoveloDireito.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_ombro_direito_Scroll(object sender, EventArgs e)
        {
            lblOmbroDireito.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_quadril_direito_Scroll(object sender, EventArgs e)
        {
            lblQuadrilDireito.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_joelho_direito_1_Scroll(object sender, EventArgs e)
        {
            lblJoelhoDireito1.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_joelho_direito_2_Scroll(object sender, EventArgs e)
        {
            lblJoelhoDireito2.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_tornozelo_direito_Scroll(object sender, EventArgs e)
        {
            lblTornozeloDireito.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_pe_direito_Scroll(object sender, EventArgs e)
        {
            lblPeDireito.Text = ((TrackBar)sender).Value.ToString();
        }

        private void trackBar_mao_esquerda_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("M_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_cotovelo_esquerdo_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("C_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_ombro_esquerdo_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("O_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_quadril_esquerdo_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("Q_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_joelho_esquerdo_1_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("1_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_joelho_esquerdo_2_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("2_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_tornozelo_esquerdo_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("T_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_pe_esquerdo_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("P_E" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_mao_direita_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("M_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_cotovelo_direito_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("C_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_ombro_direito_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("O_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_quadril_direito_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("Q_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_joelho_direito_1_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("1_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_joelho_direito_2_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("2_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_tornozelo_direito_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("T_D" + ((TrackBar)sender).Value.ToString());
        }

        private void trackBar_pe_direito_MouseUp(object sender, MouseEventArgs e)
        {
            SendSerialData("P_D" + ((TrackBar)sender).Value.ToString());
        }

        private void cmdAdd_Click(object sender, EventArgs e)
        {
            int[] s = new int[16];

            s[0] = trackBar_mao_esquerda.Value;
            s[1] = trackBar_cotovelo_esquerdo.Value;
            s[2] = trackBar_ombro_esquerdo.Value;
            s[3] = trackBar_mao_direita.Value;
            s[4] = trackBar_cotovelo_direito.Value;
            s[5] = trackBar_ombro_direito.Value;
            s[6] = trackBar_quadril_esquerdo.Value;
            s[7] = trackBar_joelho_esquerdo_1.Value;
            s[8] = trackBar_joelho_esquerdo_2.Value;
            s[9] = trackBar_tornozelo_esquerdo.Value;
            s[10] = trackBar_pe_esquerdo.Value;
            s[11] = trackBar_quadril_direito.Value;
            s[12] = trackBar_joelho_direito_1.Value;
            s[13] = trackBar_joelho_direito_2.Value;
            s[14] = trackBar_tornozelo_direito.Value;
            s[15] = trackBar_pe_direito.Value;

            CsvData data = new CsvData();
            data.values = s;
            data.name = txtName.Text;

            listPositions.Add(data);

        }


        private void LoadList()
        {
           

            using (StreamReader reader = new StreamReader(sPath))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    String[] s = line.Split(',');

                    int[] i = new int[16];

                    CsvData item = new CsvData();
                    item.name = s[0];
                    i[0] = Int32.Parse(s[1]);
                    i[1] = Int32.Parse(s[2]);
                    i[2] = Int32.Parse(s[3]);
                    i[3] = Int32.Parse(s[4]);
                    i[4] = Int32.Parse(s[5]);
                    i[5] = Int32.Parse(s[6]);
                    i[6] = Int32.Parse(s[7]);
                    i[7] = Int32.Parse(s[8]);
                    i[8] = Int32.Parse(s[9]);
                    i[9] = Int32.Parse(s[10]);
                    i[10] = Int32.Parse(s[11]);
                    i[11] = Int32.Parse(s[12]);
                    i[12] = Int32.Parse(s[13]);
                    i[13] = Int32.Parse(s[14]);
                    i[14] = Int32.Parse(s[15]);
                    i[15] = Int32.Parse(s[16]);

                    item.values = i;

                    listPositions.Add(item);

                    //list.Add(line); // Add to list.
                    Console.WriteLine(line); // Write to console.
                }
            }


            System.IO.StreamWriter SaveFile = new System.IO.StreamWriter(sPath);
            foreach (CsvData item in listBox1.Items)
            {
                SaveFile.WriteLine(item.name + item.GetSensorValues());
            }

            SaveFile.Close();

            MessageBox.Show("Programs saved!");
        }

        private void SaveList() {

            StreamWriter SaveFile = new StreamWriter(sPath);
            foreach (CsvData item in listBox1.Items)
            {
                SaveFile.WriteLine(item.name + item.GetSensorValues());
            }

            SaveFile.Close();

            MessageBox.Show("Programs saved!");
        }

        private void cmdCarregar_Click(object sender, EventArgs e)
        {
            CsvData item = listBox1.SelectedItems[listBox1.SelectedIndex] as CsvData;
            if (item != null)
            {
                trackBar_mao_esquerda.Value = item.values[0];
                trackBar_cotovelo_esquerdo.Value = item.values[1];
                trackBar_ombro_esquerdo.Value = item.values[2];
                trackBar_mao_direita.Value = item.values[3];
                trackBar_cotovelo_direito.Value = item.values[4];
                trackBar_ombro_direito.Value = item.values[5];
                trackBar_quadril_esquerdo.Value = item.values[6];
                trackBar_joelho_esquerdo_1.Value = item.values[7];
                trackBar_joelho_esquerdo_2.Value = item.values[8];
                trackBar_tornozelo_esquerdo.Value = item.values[9];

                trackBar_pe_esquerdo.Value = item.values[10];
                trackBar_quadril_direito.Value = item.values[11];
                trackBar_joelho_direito_1.Value = item.values[12];
                trackBar_joelho_direito_2.Value = item.values[13];
                trackBar_tornozelo_direito.Value = item.values[14];
                trackBar_pe_direito.Value = item.values[15];
            }
        }

        private void cmdExcluir_Click(object sender, EventArgs e)
        {
            //listBox1 
        }

        private void cmdSalvar_Click(object sender, EventArgs e)
        {
            SaveList();
        }

        private void cmdLoad_Click(object sender, EventArgs e)
        {
            LoadList();
        }
    }
}
