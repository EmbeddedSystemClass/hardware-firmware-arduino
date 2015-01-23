using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Management;

namespace Logger {
	public partial class MainTabPage : LoggerTabPage {
		private ConnectButton connectButton;
		private ResetButton resetButton;

		public MainTabPage() {
			InitializeComponent();

			connectButton = new ConnectButton("Connect", true, ImageResource.Connection32x32);			
			mainTabFlowLayoutPanel.Controls.Add(connectButton);

			resetButton = new ResetButton("Reset", false, ImageResource.Remove32x32);			
			mainTabFlowLayoutPanel.Controls.Add(resetButton);			
		}				

		public void SetEnabled(bool enabled) {
			foreach (Button btn in mainTabFlowLayoutPanel.Controls) {
				btn.Enabled = enabled;
			}

			connectButton.Enabled = true;
		}		

		public void RegisterPage(LoggerTabPage page, string name, Image image, bool enabled) {
			PageButton button = new PageButton(page, name, enabled, image);
			mainTabFlowLayoutPanel.Controls.Add(button);
		}

		public override void OnInitialize() {
			HomeButton.Instance.Visible = false;
			HomeButton.Instance.SetHomePage(this);
			Main.Instance.ToolStrip.Items.Add(HomeButton.Instance);
			base.OnInitialize();
		}

		public override void OnActivate() {
			HomeButton.Instance.Visible = false;
			base.OnActivate();
		}

		public override void OnDeactivate() {
			HomeButton.Instance.Visible = true;
			base.OnDeactivate();
		}
	}

	public class PageButton : Button {
		private LoggerTabPage page;

		public PageButton(LoggerTabPage page, string name, bool enabled, Image image) {
			this.page = page;
			BackColor = System.Drawing.Color.Transparent;
			Enabled = enabled;
			FlatAppearance.BorderSize = 0;
			FlatAppearance.MouseOverBackColor = Color.Transparent;
			FlatStyle = FlatStyle.Flat;
			this.Image = image; 
			Size = new Size(109, 81);
			Name = name;
			Text = name;
			TextAlign = ContentAlignment.BottomCenter;
		}

		protected override void OnClick(EventArgs e) {			
			page.OnActivate();
			base.OnClick(e);
		}
	}

	public class ConnectButton : Button {
		private ContextMenuStrip connectMenuStrip;
		
		public string PortName { get; set; }

		public ConnectButton(string name, bool enabled, Image image) {			
			BackColor = Color.Transparent;
			Enabled = enabled;
			FlatAppearance.BorderSize = 0;
			FlatAppearance.MouseOverBackColor = Color.Transparent;
			FlatStyle = FlatStyle.Flat;
			this.Image = image;
			Size = new Size(109, 81);
			Name = name;
			Text = name;
			TextAlign = ContentAlignment.BottomCenter;

			connectMenuStrip = new ContextMenuStrip();
			connectMenuStrip.ItemClicked += connectMenuStrip_ItemClicked;

			List<string> portNames = new List<string>(SerialPort.GetPortNames());
			portNames.Sort();
			
			string arduinoPort = "";
			foreach (USBDeviceInfo deviceInfo in GetUSBDevices()) {
				if (deviceInfo.Description.Contains("Arduino")) {
					arduinoPort = deviceInfo.DeviceID;
					break;
				}
			}

			foreach (string item in portNames) {
				if(item.Equals(arduinoPort)) {
					connectMenuStrip.Items.Add(item + " Arduino");
				} else {
					connectMenuStrip.Items.Add(item);
				}
			}
			ContextMenuStrip = connectMenuStrip;			
		}

		void connectMenuStrip_ItemClicked(object sender, ToolStripItemClickedEventArgs e) {			
			PortName = e.ClickedItem.Text.Replace(" Arduino", "");
			DataLogger.Instance.Connect(PortName);
		}

		protected override void OnClick(EventArgs e) {
			Point p = Location;
			p.Offset(Size.Width / 2, Size.Height / 2);
			connectMenuStrip.Show(this, p);

			base.OnClick(e);
		}

		static List<USBDeviceInfo> GetUSBDevices()
		{
			List<USBDeviceInfo> devices = new List<USBDeviceInfo>();

			// https://msdn.microsoft.com/en-us/library/dn605981(v=vs.85).aspx
			// https://msdn.microsoft.com/en-us/library/aa394413(v=vs.85).aspx

			ManagementObjectCollection collection;			
			using (var searcher = new ManagementObjectSearcher(@"Select * From Win32_SerialPort"))
			collection = searcher.Get();      

			foreach (var device in collection)
			{
				string s = (string)device.GetPropertyValue("Name");
				devices.Add(new USBDeviceInfo(
					(string)device.GetPropertyValue("DeviceID"),
					(string)device.GetPropertyValue("PNPDeviceID"),
					(string)device.GetPropertyValue("Description")
				));
			}

		  collection.Dispose();
		  return devices;
		}
	}

	public class ResetButton : Button {
		public ResetButton(string name, bool enabled, Image image) {
			BackColor = Color.Transparent;
			Enabled = enabled;
			FlatAppearance.BorderSize = 0;
			FlatAppearance.MouseOverBackColor = Color.Transparent;
			FlatStyle = FlatStyle.Flat;
			this.Image = image;
			Size = new Size(109, 81);
			Name = name;
			Text = name;
			TextAlign = ContentAlignment.BottomCenter;			
		}		

		protected override void OnClick(EventArgs e) {
			DialogResult dr = MessageBox.Show(
				Main.Instance, "ResetButton logger", "Reset", 
				MessageBoxButtons.YesNo, MessageBoxIcon.Question
			);
			if(dr == DialogResult.OK)
				DataLogger.Instance.ResetLooger();
			base.OnClick(e);
		}
	}	

	class USBDeviceInfo {
		public USBDeviceInfo(string deviceID, string pnpDeviceID, string description)
		{
		  this.DeviceID = deviceID;
		  this.PnpDeviceID = pnpDeviceID;
		  this.Description = description;
		}
		public string DeviceID { get; private set; }
		public string PnpDeviceID { get; private set; }
		public string Description { get; private set; }
	}

}

