using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Logger {
	public partial class LoggerTabPage : UserControl {		
		public LoggerTabPage() {
			InitializeComponent();
		}

		public virtual void OnInitialize() {
		}

		public virtual void OnActivate() {
			TabPage tp = Parent as TabPage;			
			if (tp != null) {
				TabControl tc = (TabControl)tp.Parent;
				TabPage current = tc.TabPages[tc.SelectedIndex];
				LoggerTabPage currentPage = current.Controls[0] as LoggerTabPage;
				if (currentPage != null)
					currentPage.OnDeactivate();

				tc.SelectedIndex = tc.TabPages.IndexOf(tp);				
			}
		}

		public virtual void OnDeactivate() {
		}
	}

	public class HomeButton : ToolStripButton {
		private ToolStripSeparator separatorButton;
		private LoggerTabPage page;

		public static HomeButton Instance = new HomeButton();

		public void SetHomePage(LoggerTabPage page) {
			this.page = page;			
		}
		
		public void AddTo(ToolStrip toolStrip, bool addSeparator) {
			if (addSeparator) {
				toolStrip.Items.Add(separatorButton);
			}

			toolStrip.Items.Add(this);
		}

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);

		}

		public HomeButton() {
			separatorButton = new ToolStripSeparator();
			Image = ImageResource.Back16x16;
		}

		protected override void OnClick(EventArgs e) {
			page.OnActivate();			
			base.OnClick(e);
		}
	}

	public class ClearButton : ToolStripButton {
		private ToolStripSeparator separatorButton;

		public static ClearButton Instance = new ClearButton();

		public void AddTo(ToolStrip toolStrip, bool addSeparator) {
			if (addSeparator) {
				toolStrip.Items.Add(separatorButton);
			} 
				
			toolStrip.Items.Add(this);
		}

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);
			
		}

		public ClearButton() {			
			separatorButton = new ToolStripSeparator();
			Image = ImageResource.Clear16x16;
		}
	}

	public class RefreshButton : ToolStripSplitButton {
		private ToolStripSeparator separatorButton;
		private ToolStripMenuItem timerButton;
		private ToolStripTextBox intervalTextBox;
		private Timer timer;

		public event EventHandler Refresh;

		public static RefreshButton Instance = new RefreshButton();

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(separatorButton);
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);
			Instance.TimerEnabled = false;
		}

		public RefreshButton() {
			separatorButton = new ToolStripSeparator();
			Image = ImageResource.RefreshBlue16x16;

			timerButton = new ToolStripMenuItem("Timer");
			timerButton.CheckOnClick = true;
			timerButton.CheckedChanged += timerButton_CheckedChanged;

			intervalTextBox = new ToolStripTextBox("Interval");
			intervalTextBox.Text = "1000";
			intervalTextBox.TextChanged += intervalTextBox_TextChanged;
			//intervalTextBox.Enabled = timerButton.Checked;

			DropDownItems.Add(timerButton);
			DropDownItems.Add(intervalTextBox);
			
			timer = new Timer();
			timer.Tick += timer_Tick;
			updateTimer();		
		}

		public bool TimerEnabled {
			get { return timer.Enabled; }
			set { timer.Enabled = value; }
		}

		public bool TimerButtonEnabled {
			get { return timerButton.Enabled; }
			set { 
				timerButton.Enabled = value;
				intervalTextBox.Enabled = value;
			}	
		}

		private void updateTimer() {
			int interval = 0;
			if (int.TryParse(intervalTextBox.Text, out interval)) {
				timer.Interval = interval >= 500 ? interval : 1000;
			}

			timer.Enabled = timerButton.Checked;
		}

		private void intervalTextBox_TextChanged(object sender, EventArgs e) {
			updateTimer();
		}

		private void timerButton_CheckedChanged(object sender, EventArgs e) {
			//intervalTextBox.Enabled = timerButton.Checked;
			updateTimer();
		}

		private void onRefresh() {
			if(!DropDownButtonPressed && Refresh != null)
				Refresh(this, EventArgs.Empty);
		}

		protected override void OnClick(EventArgs e) {
			base.OnClick(e);

			onRefresh();
		}

		void timer_Tick(object sender, EventArgs e) {
			onRefresh();
		}
	}

	public class SensorButton : ToolStripButton {
		private ToolStripSeparator separatorButton;
		private ToolStripButton enableAllButton;

		public static List<SensorButton> Instances = new List<SensorButton>();

		public static void AddSensors(List<SensorItem> sensors) {
			foreach (SensorItem sensor in sensors) {
				SensorButton btn = new SensorButton();
				btn.SensorId = sensor.Id;
				btn.Name = sensor.Id.ToString();
				btn.Text = sensor.Name;				
				Instances.Add(btn);
			}
		}

		public static void Clear() {
			Instances.Clear();
		}

		public static void AddTo(ToolStrip toolStrip) {
			if (Instances.Count == 0)
				return;
			toolStrip.Items.Add(Instances[0].separatorButton);
			foreach (SensorButton item in Instances) {
				toolStrip.Items.Add(item);
				item.Checked = true;
			}
			toolStrip.Items.Add(Instances[0].enableAllButton);
			Instances[0].enableAllButton.Checked = true;
		}

		public static void RemoveFrom(ToolStrip toolStrip) {
			if (Instances.Count == 0)
				return;
			toolStrip.Items.Remove(Instances[0].separatorButton);
			foreach (SensorButton item in Instances) {
				toolStrip.Items.Remove(item);
			}

			toolStrip.Items.Remove(Instances[0].enableAllButton);
		}

		public SensorButton() {
			separatorButton = new ToolStripSeparator();
			enableAllButton = new ToolStripButton();
			enableAllButton.CheckOnClick = true;
			enableAllButton.Image = ImageResource.Multiply16x16;
			enableAllButton.CheckStateChanged += enableAllButton_CheckStateChanged;
			enableAllButton.ToolTipText = "Check/Uncheck all";
			Image = ImageResource.Temperature16x16;
			CheckOnClick = true;
		}

		void enableAllButton_CheckStateChanged(object sender, EventArgs e) {
			foreach (var item in Instances) {
				item.Checked = enableAllButton.Checked;
			}
		}

		public byte SensorId { get; set; }
	}

	public class ProgressBar : ToolStripProgressBar {
		private ToolStripSeparator separatorButton;
		private ToolStripButton cancelButton;

		public static ProgressBar Instance = new ProgressBar();

		public event EventHandler Cancel;

		public void AddTo(ToolStrip toolStrip, bool addSeparator) {
			if (addSeparator) {
				toolStrip.Items.Add(separatorButton);
			}

			toolStrip.Items.Add(this);
			toolStrip.Items.Add(cancelButton);
		}

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);
			toolStrip.Items.Remove(cancelButton);
		}

		public ProgressBar() {
			separatorButton = new ToolStripSeparator();
			
			cancelButton = new ToolStripButton("Cancel");
			cancelButton.Click += cancelButton_Click;

			Style = ProgressBarStyle.Marquee;			
		}

		private void cancelButton_Click(object sender, EventArgs e) {
			if (Cancel != null) {
				Cancel(this, EventArgs.Empty);
			}			
		}

		public new bool CancelButtonVisible {
			get { return cancelButton.Visible; }
			set {
				cancelButton.Visible = value;
			}
		}

		public new bool Visible {
			get { return base.Visible; }
			set { 
				base.Visible = value;
				cancelButton.Visible = value;
				separatorButton.Visible = value;
			}
		}
	}
}
