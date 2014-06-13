using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Logger
{
    public partial class Main : Form
    {
		public Main()
        {
			InitializeComponent();

			Instance = this;

			mainTab.SelectedIndex = 0;
			mainTab.ItemSize = new Size(0, 1);
			mainTab.SizeMode = TabSizeMode.Fixed;

			mainTabPageControl.RegisterPage(dateTimePageControl, "Date Time", ImageResource.Clock32x32, false);
			mainTabPageControl.RegisterPage(temperatureTabPageControl, "Temperature", ImageResource.Cab32x32, false);
			mainTabPageControl.RegisterPage(temperatureChartTabPageControl, "Temperature chart", ImageResource.Chart32x32, false);
			mainTabPageControl.RegisterPage(logFilesTabPageControl, "Log files", ImageResource.DataBase32x32, false);

			mainTabPageControl.SetEnabled(false);

			DataLogger.Instance.OnConnectionChanged += Logger_OnConnectionChanged;

			mainTabPageControl.OnInitialize();

			updateUi();
        }

		public static Main Instance { get; set; }

		public ToolStrip ToolStrip { get { return mainToolStrip; } }

		void Logger_OnConnectionChanged(object sender, EventArgs e) {
			updateUi();
		}

		private void updateUi() {
			mainTabPageControl.SetEnabled(DataLogger.Instance.IsConnected);		
		}					

		private void homeToolStripButton_Click(object sender, EventArgs e) {
			mainTab.SelectedIndex = 0;
		}	

		public void ActivateTab(int index) {
			mainTab.SelectedIndex = index;
		}						
    }
}
