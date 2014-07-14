using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

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
			Main.Instance.ToolStrip.Items.Add(new HomeButton(this));
			base.OnInitialize();
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

			foreach (string item in SerialPort.GetPortNames()) {
				connectMenuStrip.Items.Add(item);
			}
			ContextMenuStrip = connectMenuStrip;			
		}

		void connectMenuStrip_ItemClicked(object sender, ToolStripItemClickedEventArgs e) {			
			PortName = e.ClickedItem.Text;
			DataLogger.Instance.Connect(PortName);
		}

		protected override void OnClick(EventArgs e) {
			Point p = Location;
			p.Offset(Size.Width / 2, Size.Height / 2);
			connectMenuStrip.Show(this, p);

			base.OnClick(e);
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

	public class HomeButton : ToolStripButton {
		private LoggerTabPage page;

		public HomeButton(LoggerTabPage page) {
			this.page = page;
			DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.ImageAndText;
			Image = ImageResource.Back16x16;
			ImageTransparentColor = Color.Magenta;
			Name = "Home";
			Size = new Size(23, 22);
			Text = "Home";		
		}

		protected override void OnClick(EventArgs e) {
			page.OnActivate();
			base.OnClick(e);
		}
	}
}
