namespace Logger
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.mainToolStrip = new System.Windows.Forms.ToolStrip();
			this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
			this.mainTab = new System.Windows.Forms.TabControl();
			this.mainTabPage = new System.Windows.Forms.TabPage();
			this.currentTabPage = new System.Windows.Forms.TabPage();
			this.chartTabPage = new System.Windows.Forms.TabPage();
			this.logFilesTabPage = new System.Windows.Forms.TabPage();
			this.dateTimePage = new System.Windows.Forms.TabPage();
			this.mainTabPageControl = new Logger.MainTabPage();
			this.currentTabPageControl = new Logger.CurrentTabPage();
			this.chartTabPageControl = new Logger.ChartTabPage();
			this.logFilesTabPageControl = new Logger.LogFilesTabPage();
			this.dateTimePageControl = new Logger.DateTimeTabPage();
			this.mainTab.SuspendLayout();
			this.mainTabPage.SuspendLayout();
			this.currentTabPage.SuspendLayout();
			this.chartTabPage.SuspendLayout();
			this.logFilesTabPage.SuspendLayout();
			this.dateTimePage.SuspendLayout();
			this.SuspendLayout();
			// 
			// mainToolStrip
			// 
			this.mainToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.mainToolStrip.Location = new System.Drawing.Point(0, 0);
			this.mainToolStrip.Name = "mainToolStrip";
			this.mainToolStrip.Size = new System.Drawing.Size(422, 25);
			this.mainToolStrip.TabIndex = 13;
			this.mainToolStrip.Text = "toolStrip1";
			// 
			// toolStripSeparator
			// 
			this.toolStripSeparator.Name = "toolStripSeparator";
			this.toolStripSeparator.Size = new System.Drawing.Size(6, 25);
			// 
			// mainTab
			// 
			this.mainTab.Appearance = System.Windows.Forms.TabAppearance.FlatButtons;
			this.mainTab.Controls.Add(this.mainTabPage);
			this.mainTab.Controls.Add(this.currentTabPage);
			this.mainTab.Controls.Add(this.chartTabPage);
			this.mainTab.Controls.Add(this.logFilesTabPage);
			this.mainTab.Controls.Add(this.dateTimePage);
			this.mainTab.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainTab.ItemSize = new System.Drawing.Size(0, 21);
			this.mainTab.Location = new System.Drawing.Point(0, 25);
			this.mainTab.Margin = new System.Windows.Forms.Padding(0);
			this.mainTab.Name = "mainTab";
			this.mainTab.Padding = new System.Drawing.Point(0, 0);
			this.mainTab.SelectedIndex = 0;
			this.mainTab.Size = new System.Drawing.Size(422, 359);
			this.mainTab.TabIndex = 14;
			// 
			// mainTabPage
			// 
			this.mainTabPage.BackColor = System.Drawing.Color.White;
			this.mainTabPage.Controls.Add(this.mainTabPageControl);
			this.mainTabPage.Location = new System.Drawing.Point(4, 25);
			this.mainTabPage.Margin = new System.Windows.Forms.Padding(0);
			this.mainTabPage.Name = "mainTabPage";
			this.mainTabPage.Size = new System.Drawing.Size(414, 330);
			this.mainTabPage.TabIndex = 3;
			this.mainTabPage.Text = "mainTabPage";
			this.mainTabPage.UseVisualStyleBackColor = true;
			// 
			// currentTabPage
			// 
			this.currentTabPage.BackColor = System.Drawing.Color.White;
			this.currentTabPage.Controls.Add(this.currentTabPageControl);
			this.currentTabPage.Location = new System.Drawing.Point(4, 25);
			this.currentTabPage.Margin = new System.Windows.Forms.Padding(0);
			this.currentTabPage.Name = "currentTabPage";
			this.currentTabPage.Size = new System.Drawing.Size(397, 330);
			this.currentTabPage.TabIndex = 1;
			this.currentTabPage.Text = "Temperature";
			this.currentTabPage.UseVisualStyleBackColor = true;
			// 
			// chartTabPage
			// 
			this.chartTabPage.BackColor = System.Drawing.Color.White;
			this.chartTabPage.Controls.Add(this.chartTabPageControl);
			this.chartTabPage.Location = new System.Drawing.Point(4, 25);
			this.chartTabPage.Margin = new System.Windows.Forms.Padding(0);
			this.chartTabPage.Name = "chartTabPage";
			this.chartTabPage.Size = new System.Drawing.Size(397, 330);
			this.chartTabPage.TabIndex = 0;
			this.chartTabPage.Text = "Log Chart";
			this.chartTabPage.UseVisualStyleBackColor = true;
			// 
			// logFilesTabPage
			// 
			this.logFilesTabPage.BackColor = System.Drawing.Color.White;
			this.logFilesTabPage.Controls.Add(this.logFilesTabPageControl);
			this.logFilesTabPage.Location = new System.Drawing.Point(4, 25);
			this.logFilesTabPage.Margin = new System.Windows.Forms.Padding(0);
			this.logFilesTabPage.Name = "logFilesTabPage";
			this.logFilesTabPage.Size = new System.Drawing.Size(397, 330);
			this.logFilesTabPage.TabIndex = 2;
			this.logFilesTabPage.Text = "Log files";
			this.logFilesTabPage.UseVisualStyleBackColor = true;
			// 
			// dateTimePage
			// 
			this.dateTimePage.Controls.Add(this.dateTimePageControl);
			this.dateTimePage.Location = new System.Drawing.Point(4, 25);
			this.dateTimePage.Name = "dateTimePage";
			this.dateTimePage.Padding = new System.Windows.Forms.Padding(3);
			this.dateTimePage.Size = new System.Drawing.Size(397, 330);
			this.dateTimePage.TabIndex = 4;
			this.dateTimePage.Text = "tabPage1";
			this.dateTimePage.UseVisualStyleBackColor = true;
			// 
			// mainTabPageControl
			// 
			this.mainTabPageControl.AutoSize = true;
			this.mainTabPageControl.BackColor = System.Drawing.Color.White;
			this.mainTabPageControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainTabPageControl.Location = new System.Drawing.Point(0, 0);
			this.mainTabPageControl.Name = "mainTabPageControl";
			this.mainTabPageControl.Size = new System.Drawing.Size(414, 330);
			this.mainTabPageControl.TabIndex = 0;
			// 
			// currentTabPageControl
			// 
			this.currentTabPageControl.AutoSize = true;
			this.currentTabPageControl.BackColor = System.Drawing.Color.White;
			this.currentTabPageControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.currentTabPageControl.Location = new System.Drawing.Point(0, 0);
			this.currentTabPageControl.Name = "currentTabPageControl";
			this.currentTabPageControl.Size = new System.Drawing.Size(397, 330);
			this.currentTabPageControl.TabIndex = 0;
			// 
			// chartTabPageControl
			// 
			this.chartTabPageControl.AutoSize = true;
			this.chartTabPageControl.BackColor = System.Drawing.Color.White;
			this.chartTabPageControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.chartTabPageControl.Location = new System.Drawing.Point(0, 0);
			this.chartTabPageControl.Name = "chartTabPageControl";
			this.chartTabPageControl.Size = new System.Drawing.Size(397, 330);
			this.chartTabPageControl.TabIndex = 0;
			// 
			// logFilesTabPageControl
			// 
			this.logFilesTabPageControl.AutoSize = true;
			this.logFilesTabPageControl.BackColor = System.Drawing.Color.White;
			this.logFilesTabPageControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.logFilesTabPageControl.Location = new System.Drawing.Point(0, 0);
			this.logFilesTabPageControl.Name = "logFilesTabPageControl";
			this.logFilesTabPageControl.Size = new System.Drawing.Size(397, 330);
			this.logFilesTabPageControl.TabIndex = 0;
			// 
			// dateTimePageControl
			// 
			this.dateTimePageControl.AutoSize = true;
			this.dateTimePageControl.BackColor = System.Drawing.Color.White;
			this.dateTimePageControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dateTimePageControl.Location = new System.Drawing.Point(3, 3);
			this.dateTimePageControl.Name = "dateTimePageControl";
			this.dateTimePageControl.Size = new System.Drawing.Size(391, 324);
			this.dateTimePageControl.TabIndex = 0;
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(422, 384);
			this.Controls.Add(this.mainTab);
			this.Controls.Add(this.mainToolStrip);
			this.Name = "Main";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Logger";
			this.mainTab.ResumeLayout(false);
			this.mainTabPage.ResumeLayout(false);
			this.mainTabPage.PerformLayout();
			this.currentTabPage.ResumeLayout(false);
			this.currentTabPage.PerformLayout();
			this.chartTabPage.ResumeLayout(false);
			this.chartTabPage.PerformLayout();
			this.logFilesTabPage.ResumeLayout(false);
			this.logFilesTabPage.PerformLayout();
			this.dateTimePage.ResumeLayout(false);
			this.dateTimePage.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

		private System.Windows.Forms.ToolStrip mainToolStrip;
		private System.Windows.Forms.TabControl mainTab;
		private System.Windows.Forms.TabPage mainTabPage;
		private System.Windows.Forms.TabPage currentTabPage;
		private System.Windows.Forms.TabPage chartTabPage;
		private System.Windows.Forms.TabPage logFilesTabPage;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
		private System.Windows.Forms.TabPage dateTimePage;
		private DateTimeTabPage dateTimePageControl;
		private LogFilesTabPage logFilesTabPageControl;
		private ChartTabPage chartTabPageControl;
		private CurrentTabPage currentTabPageControl;
		private MainTabPage mainTabPageControl;
    }
}

