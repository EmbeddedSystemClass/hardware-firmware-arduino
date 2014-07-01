namespace Logger {
	partial class MainTabPage {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.mainTabFlowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
			this.SuspendLayout();
			// 
			// mainTabFlowLayoutPanel
			// 
			this.mainTabFlowLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainTabFlowLayoutPanel.Location = new System.Drawing.Point(0, 0);
			this.mainTabFlowLayoutPanel.Margin = new System.Windows.Forms.Padding(0);
			this.mainTabFlowLayoutPanel.Name = "mainTabFlowLayoutPanel";
			this.mainTabFlowLayoutPanel.Size = new System.Drawing.Size(487, 341);
			this.mainTabFlowLayoutPanel.TabIndex = 3;
			// 
			// MainTabPage
			// 
			this.BackColor = System.Drawing.Color.White;
			this.Controls.Add(this.mainTabFlowLayoutPanel);
			this.Name = "MainTabPage";
			this.Size = new System.Drawing.Size(487, 341);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.FlowLayoutPanel mainTabFlowLayoutPanel;
	}
}
