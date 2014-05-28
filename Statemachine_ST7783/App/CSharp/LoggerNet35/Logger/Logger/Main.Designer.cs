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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
			System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
			this.getTemperatureLogButton = new System.Windows.Forms.Button();
			this.temperatureChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
			this.chartMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.printMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.getDirectoryButton = new System.Windows.Forms.Button();
			this.mainTab = new System.Windows.Forms.TabControl();
			this.mainTabPage = new System.Windows.Forms.TabPage();
			this.mainTabFlowLayoutPanel = new System.Windows.Forms.FlowLayoutPanel();
			this.connectButton = new System.Windows.Forms.Button();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.dateTimeButton = new System.Windows.Forms.Button();
			this.logFilesButton = new System.Windows.Forms.Button();
			this.chartButton = new System.Windows.Forms.Button();
			this.resetButton = new System.Windows.Forms.Button();
			this.temperatureButton = new System.Windows.Forms.Button();
			this.temperatureTabPage = new System.Windows.Forms.TabPage();
			this.button2 = new System.Windows.Forms.Button();
			this.getTemperatureButton = new System.Windows.Forms.Button();
			this.temperatureLabel = new System.Windows.Forms.Label();
			this.logChartTabPage = new System.Windows.Forms.TabPage();
			this.button1 = new System.Windows.Forms.Button();
			this.logFilesTabPage = new System.Windows.Forms.TabPage();
			this.homeButton = new System.Windows.Forms.Button();
			this.logFileValuesListView = new System.Windows.Forms.ListView();
			this.Values = new System.Windows.Forms.ColumnHeader();
			this.logFilesListView = new System.Windows.Forms.ListView();
			this.File = new System.Windows.Forms.ColumnHeader();
			this.connectMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.temperatureChart)).BeginInit();
			this.chartMenuStrip.SuspendLayout();
			this.mainTab.SuspendLayout();
			this.mainTabPage.SuspendLayout();
			this.mainTabFlowLayoutPanel.SuspendLayout();
			this.temperatureTabPage.SuspendLayout();
			this.logChartTabPage.SuspendLayout();
			this.logFilesTabPage.SuspendLayout();
			this.connectMenuStrip.SuspendLayout();
			this.SuspendLayout();
			// 
			// getTemperatureLogButton
			// 
			this.getTemperatureLogButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.getTemperatureLogButton.Location = new System.Drawing.Point(455, 6);
			this.getTemperatureLogButton.Name = "getTemperatureLogButton";
			this.getTemperatureLogButton.Size = new System.Drawing.Size(95, 23);
			this.getTemperatureLogButton.TabIndex = 7;
			this.getTemperatureLogButton.Text = "Get Log";
			this.getTemperatureLogButton.UseVisualStyleBackColor = true;
			this.getTemperatureLogButton.Click += new System.EventHandler(this.getTemperatureLogButton_Click);
			// 
			// temperatureChart
			// 
			this.temperatureChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.temperatureChart.BorderlineColor = System.Drawing.Color.Black;
			this.temperatureChart.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
			chartArea1.AxisX.MajorGrid.Enabled = false;
			chartArea1.AxisX.Title = "Hours";
			chartArea1.AxisY.TextOrientation = System.Windows.Forms.DataVisualization.Charting.TextOrientation.Horizontal;
			chartArea1.AxisY.Title = "°C";
			chartArea1.Name = "ChartArea1";
			this.temperatureChart.ChartAreas.Add(chartArea1);
			this.temperatureChart.ContextMenuStrip = this.chartMenuStrip;
			this.temperatureChart.Location = new System.Drawing.Point(6, 6);
			this.temperatureChart.Name = "temperatureChart";
			series1.ChartArea = "ChartArea1";
			series1.Name = "Series1";
			this.temperatureChart.Series.Add(series1);
			this.temperatureChart.Size = new System.Drawing.Size(443, 368);
			this.temperatureChart.TabIndex = 8;
			this.temperatureChart.Text = "chart1";
			title1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
			title1.Name = "Title1";
			title1.Text = "Temperature";
			this.temperatureChart.Titles.Add(title1);
			// 
			// chartMenuStrip
			// 
			this.chartMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.printMenuItem});
			this.chartMenuStrip.Name = "chartMenuStrip";
			this.chartMenuStrip.Size = new System.Drawing.Size(100, 26);
			// 
			// printMenuItem
			// 
			this.printMenuItem.Name = "printMenuItem";
			this.printMenuItem.Size = new System.Drawing.Size(99, 22);
			this.printMenuItem.Text = "Print";
			this.printMenuItem.Click += new System.EventHandler(this.printMenuItem_Click);
			// 
			// getDirectoryButton
			// 
			this.getDirectoryButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.getDirectoryButton.Location = new System.Drawing.Point(455, 6);
			this.getDirectoryButton.Name = "getDirectoryButton";
			this.getDirectoryButton.Size = new System.Drawing.Size(95, 23);
			this.getDirectoryButton.TabIndex = 11;
			this.getDirectoryButton.Text = "Get Dir";
			this.getDirectoryButton.UseVisualStyleBackColor = true;
			this.getDirectoryButton.Click += new System.EventHandler(this.getDirectoryButton_Click);
			// 
			// mainTab
			// 
			this.mainTab.Appearance = System.Windows.Forms.TabAppearance.FlatButtons;
			this.mainTab.Controls.Add(this.mainTabPage);
			this.mainTab.Controls.Add(this.temperatureTabPage);
			this.mainTab.Controls.Add(this.logChartTabPage);
			this.mainTab.Controls.Add(this.logFilesTabPage);
			this.mainTab.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainTab.ItemSize = new System.Drawing.Size(0, 21);
			this.mainTab.Location = new System.Drawing.Point(0, 0);
			this.mainTab.Margin = new System.Windows.Forms.Padding(0);
			this.mainTab.Name = "mainTab";
			this.mainTab.SelectedIndex = 0;
			this.mainTab.Size = new System.Drawing.Size(564, 409);
			this.mainTab.TabIndex = 12;
			// 
			// mainTabPage
			// 
			this.mainTabPage.BackColor = System.Drawing.Color.White;
			this.mainTabPage.Controls.Add(this.mainTabFlowLayoutPanel);
			this.mainTabPage.Location = new System.Drawing.Point(4, 25);
			this.mainTabPage.Name = "mainTabPage";
			this.mainTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.mainTabPage.Size = new System.Drawing.Size(556, 380);
			this.mainTabPage.TabIndex = 3;
			this.mainTabPage.Text = "mainTabPage";
			this.mainTabPage.UseVisualStyleBackColor = true;
			// 
			// mainTabFlowLayoutPanel
			// 
			this.mainTabFlowLayoutPanel.Controls.Add(this.connectButton);
			this.mainTabFlowLayoutPanel.Controls.Add(this.dateTimeButton);
			this.mainTabFlowLayoutPanel.Controls.Add(this.logFilesButton);
			this.mainTabFlowLayoutPanel.Controls.Add(this.chartButton);
			this.mainTabFlowLayoutPanel.Controls.Add(this.resetButton);
			this.mainTabFlowLayoutPanel.Controls.Add(this.temperatureButton);
			this.mainTabFlowLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainTabFlowLayoutPanel.Location = new System.Drawing.Point(3, 3);
			this.mainTabFlowLayoutPanel.Name = "mainTabFlowLayoutPanel";
			this.mainTabFlowLayoutPanel.Size = new System.Drawing.Size(550, 374);
			this.mainTabFlowLayoutPanel.TabIndex = 2;
			// 
			// connectButton
			// 
			this.connectButton.BackColor = System.Drawing.Color.Transparent;
			this.connectButton.Enabled = false;
			this.connectButton.FlatAppearance.BorderSize = 0;
			this.connectButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
			this.connectButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.connectButton.ImageIndex = 2;
			this.connectButton.ImageList = this.imageList1;
			this.connectButton.Location = new System.Drawing.Point(3, 3);
			this.connectButton.Name = "connectButton";
			this.connectButton.Size = new System.Drawing.Size(109, 81);
			this.connectButton.TabIndex = 1;
			this.connectButton.Text = "Connection";
			this.connectButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.connectButton.UseVisualStyleBackColor = false;
			this.connectButton.Click += new System.EventHandler(this.tabSelectButton_Click);
			// 
			// imageList1
			// 
			this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			this.imageList1.Images.SetKeyName(0, "Cab32x32.png");
			this.imageList1.Images.SetKeyName(1, "ChartTypeAreaInsertGallery32x32.png");
			this.imageList1.Images.SetKeyName(2, "ConnectUSB32.ico");
			this.imageList1.Images.SetKeyName(3, "DatabaseSqlServer.png");
			this.imageList1.Images.SetKeyName(4, "DataRefreshAll.png");
			this.imageList1.Images.SetKeyName(5, "FileOpen.png");
			this.imageList1.Images.SetKeyName(6, "HeaderFooterCurrentTimeInsert32x32.png");
			this.imageList1.Images.SetKeyName(7, "Ieframe 6.ico");
			this.imageList1.Images.SetKeyName(8, "ImportMoreMenu.png");
			this.imageList1.Images.SetKeyName(9, "kcsRemove32.png");
			this.imageList1.Images.SetKeyName(10, "kcsUpdateData.png");
			this.imageList1.Images.SetKeyName(11, "QueryDelete.png");
			this.imageList1.Images.SetKeyName(12, "USB_Silver 32x32.ico");
			// 
			// dateTimeButton
			// 
			this.dateTimeButton.BackColor = System.Drawing.Color.Transparent;
			this.dateTimeButton.Enabled = false;
			this.dateTimeButton.FlatAppearance.BorderSize = 0;
			this.dateTimeButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
			this.dateTimeButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.dateTimeButton.ImageIndex = 6;
			this.dateTimeButton.ImageList = this.imageList1;
			this.dateTimeButton.Location = new System.Drawing.Point(118, 3);
			this.dateTimeButton.Name = "dateTimeButton";
			this.dateTimeButton.Size = new System.Drawing.Size(109, 81);
			this.dateTimeButton.TabIndex = 2;
			this.dateTimeButton.Text = "Date Time";
			this.dateTimeButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.dateTimeButton.UseVisualStyleBackColor = false;
			this.dateTimeButton.Click += new System.EventHandler(this.tabSelectButton_Click);
			// 
			// logFilesButton
			// 
			this.logFilesButton.BackColor = System.Drawing.Color.Transparent;
			this.logFilesButton.Enabled = false;
			this.logFilesButton.FlatAppearance.BorderSize = 0;
			this.logFilesButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
			this.logFilesButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
			this.logFilesButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.logFilesButton.ImageKey = "ImportMoreMenu.png";
			this.logFilesButton.ImageList = this.imageList1;
			this.logFilesButton.Location = new System.Drawing.Point(233, 3);
			this.logFilesButton.Name = "logFilesButton";
			this.logFilesButton.Size = new System.Drawing.Size(109, 81);
			this.logFilesButton.TabIndex = 3;
			this.logFilesButton.Text = "Log files";
			this.logFilesButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.logFilesButton.UseVisualStyleBackColor = false;
			this.logFilesButton.Click += new System.EventHandler(this.tabSelectButton_Click);
			// 
			// chartButton
			// 
			this.chartButton.BackColor = System.Drawing.Color.Transparent;
			this.chartButton.Enabled = false;
			this.chartButton.FlatAppearance.BorderSize = 0;
			this.chartButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
			this.chartButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
			this.chartButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.chartButton.ImageKey = "ChartTypeAreaInsertGallery32x32.png";
			this.chartButton.ImageList = this.imageList1;
			this.chartButton.Location = new System.Drawing.Point(348, 3);
			this.chartButton.Name = "chartButton";
			this.chartButton.Size = new System.Drawing.Size(109, 81);
			this.chartButton.TabIndex = 4;
			this.chartButton.Text = "Chart";
			this.chartButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.chartButton.UseVisualStyleBackColor = false;
			this.chartButton.Click += new System.EventHandler(this.tabSelectButton_Click);
			// 
			// resetButton
			// 
			this.resetButton.BackColor = System.Drawing.Color.Transparent;
			this.resetButton.Enabled = false;
			this.resetButton.FlatAppearance.BorderSize = 0;
			this.resetButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
			this.resetButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
			this.resetButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.resetButton.ImageKey = "kcsRemove32.png";
			this.resetButton.ImageList = this.imageList1;
			this.resetButton.Location = new System.Drawing.Point(3, 90);
			this.resetButton.Name = "resetButton";
			this.resetButton.Size = new System.Drawing.Size(109, 81);
			this.resetButton.TabIndex = 5;
			this.resetButton.Text = "Reset";
			this.resetButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.resetButton.UseVisualStyleBackColor = false;
			this.resetButton.Click += new System.EventHandler(this.tabSelectButton_Click);
			// 
			// temperatureButton
			// 
			this.temperatureButton.BackColor = System.Drawing.Color.Transparent;
			this.temperatureButton.Enabled = false;
			this.temperatureButton.FlatAppearance.BorderSize = 0;
			this.temperatureButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
			this.temperatureButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
			this.temperatureButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.temperatureButton.ImageKey = "Cab32x32.png";
			this.temperatureButton.ImageList = this.imageList1;
			this.temperatureButton.Location = new System.Drawing.Point(118, 90);
			this.temperatureButton.Name = "temperatureButton";
			this.temperatureButton.Size = new System.Drawing.Size(109, 81);
			this.temperatureButton.TabIndex = 6;
			this.temperatureButton.Text = "Temperature";
			this.temperatureButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.temperatureButton.UseVisualStyleBackColor = false;
			this.temperatureButton.Click += new System.EventHandler(this.tabSelectButton_Click);
			// 
			// temperatureTabPage
			// 
			this.temperatureTabPage.BackColor = System.Drawing.Color.White;
			this.temperatureTabPage.Controls.Add(this.button2);
			this.temperatureTabPage.Controls.Add(this.getTemperatureButton);
			this.temperatureTabPage.Controls.Add(this.temperatureLabel);
			this.temperatureTabPage.Location = new System.Drawing.Point(4, 25);
			this.temperatureTabPage.Name = "temperatureTabPage";
			this.temperatureTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.temperatureTabPage.Size = new System.Drawing.Size(556, 380);
			this.temperatureTabPage.TabIndex = 1;
			this.temperatureTabPage.Text = "Temperature";
			// 
			// button2
			// 
			this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.button2.Location = new System.Drawing.Point(455, 13);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(95, 23);
			this.button2.TabIndex = 16;
			this.button2.Text = "Home";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.homeButton_Click);
			// 
			// getTemperatureButton
			// 
			this.getTemperatureButton.Location = new System.Drawing.Point(112, 13);
			this.getTemperatureButton.Name = "getTemperatureButton";
			this.getTemperatureButton.Size = new System.Drawing.Size(100, 23);
			this.getTemperatureButton.TabIndex = 1;
			this.getTemperatureButton.Text = "Get Temperature";
			this.getTemperatureButton.UseVisualStyleBackColor = true;
			this.getTemperatureButton.Click += new System.EventHandler(this.getTemperatureButton_Click);
			// 
			// temperatureLabel
			// 
			this.temperatureLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.temperatureLabel.Location = new System.Drawing.Point(6, 13);
			this.temperatureLabel.Name = "temperatureLabel";
			this.temperatureLabel.Size = new System.Drawing.Size(100, 23);
			this.temperatureLabel.TabIndex = 0;
			this.temperatureLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// logChartTabPage
			// 
			this.logChartTabPage.BackColor = System.Drawing.Color.White;
			this.logChartTabPage.Controls.Add(this.button1);
			this.logChartTabPage.Controls.Add(this.temperatureChart);
			this.logChartTabPage.Controls.Add(this.getTemperatureLogButton);
			this.logChartTabPage.Location = new System.Drawing.Point(4, 25);
			this.logChartTabPage.Name = "logChartTabPage";
			this.logChartTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.logChartTabPage.Size = new System.Drawing.Size(556, 380);
			this.logChartTabPage.TabIndex = 0;
			this.logChartTabPage.Text = "Log Chart";
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.Location = new System.Drawing.Point(455, 35);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(95, 23);
			this.button1.TabIndex = 15;
			this.button1.Text = "Home";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.homeButton_Click);
			// 
			// logFilesTabPage
			// 
			this.logFilesTabPage.BackColor = System.Drawing.Color.White;
			this.logFilesTabPage.Controls.Add(this.homeButton);
			this.logFilesTabPage.Controls.Add(this.logFileValuesListView);
			this.logFilesTabPage.Controls.Add(this.logFilesListView);
			this.logFilesTabPage.Controls.Add(this.getDirectoryButton);
			this.logFilesTabPage.Location = new System.Drawing.Point(4, 25);
			this.logFilesTabPage.Name = "logFilesTabPage";
			this.logFilesTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.logFilesTabPage.Size = new System.Drawing.Size(556, 380);
			this.logFilesTabPage.TabIndex = 2;
			this.logFilesTabPage.Text = "Log files";
			// 
			// homeButton
			// 
			this.homeButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.homeButton.Location = new System.Drawing.Point(455, 35);
			this.homeButton.Name = "homeButton";
			this.homeButton.Size = new System.Drawing.Size(95, 23);
			this.homeButton.TabIndex = 14;
			this.homeButton.Text = "Home";
			this.homeButton.UseVisualStyleBackColor = true;
			this.homeButton.Click += new System.EventHandler(this.homeButton_Click);
			// 
			// logFileValuesListView
			// 
			this.logFileValuesListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.logFileValuesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Values});
			this.logFileValuesListView.Location = new System.Drawing.Point(6, 200);
			this.logFileValuesListView.Name = "logFileValuesListView";
			this.logFileValuesListView.Size = new System.Drawing.Size(443, 174);
			this.logFileValuesListView.TabIndex = 13;
			this.logFileValuesListView.UseCompatibleStateImageBehavior = false;
			this.logFileValuesListView.View = System.Windows.Forms.View.Details;
			// 
			// Values
			// 
			this.Values.Text = "Values";
			this.Values.Width = 438;
			// 
			// logFilesListView
			// 
			this.logFilesListView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.logFilesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.File});
			this.logFilesListView.Location = new System.Drawing.Point(3, 6);
			this.logFilesListView.Name = "logFilesListView";
			this.logFilesListView.Size = new System.Drawing.Size(446, 188);
			this.logFilesListView.TabIndex = 12;
			this.logFilesListView.UseCompatibleStateImageBehavior = false;
			this.logFilesListView.View = System.Windows.Forms.View.Details;
			this.logFilesListView.DoubleClick += new System.EventHandler(this.logFilesListView_DoubleClick);
			// 
			// File
			// 
			this.File.Text = "File";
			this.File.Width = 441;
			// 
			// connectMenuStrip
			// 
			this.connectMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1});
			this.connectMenuStrip.Name = "chartMenuStrip";
			this.connectMenuStrip.Size = new System.Drawing.Size(113, 26);
			this.connectMenuStrip.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.connectMenuStrip_ItemClicked);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(112, 22);
			this.toolStripMenuItem1.Text = "Com11";
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(564, 409);
			this.Controls.Add(this.mainTab);
			this.Name = "Main";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Logger";
			((System.ComponentModel.ISupportInitialize)(this.temperatureChart)).EndInit();
			this.chartMenuStrip.ResumeLayout(false);
			this.mainTab.ResumeLayout(false);
			this.mainTabPage.ResumeLayout(false);
			this.mainTabFlowLayoutPanel.ResumeLayout(false);
			this.temperatureTabPage.ResumeLayout(false);
			this.logChartTabPage.ResumeLayout(false);
			this.logFilesTabPage.ResumeLayout(false);
			this.connectMenuStrip.ResumeLayout(false);
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.Button getTemperatureLogButton;
        private System.Windows.Forms.DataVisualization.Charting.Chart temperatureChart;
		private System.Windows.Forms.ContextMenuStrip chartMenuStrip;
		private System.Windows.Forms.ToolStripMenuItem printMenuItem;
		private System.Windows.Forms.Button getDirectoryButton;
		private System.Windows.Forms.TabControl mainTab;
		private System.Windows.Forms.TabPage temperatureTabPage;
		private System.Windows.Forms.TabPage logChartTabPage;
		private System.Windows.Forms.TabPage logFilesTabPage;
		private System.Windows.Forms.ListView logFileValuesListView;
		private System.Windows.Forms.ListView logFilesListView;
		private System.Windows.Forms.ColumnHeader File;
		private System.Windows.Forms.ColumnHeader Values;
		private System.Windows.Forms.TabPage mainTabPage;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.Button homeButton;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.ContextMenuStrip connectMenuStrip;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
		private System.Windows.Forms.Button getTemperatureButton;
		private System.Windows.Forms.Label temperatureLabel;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button connectButton;
		private System.Windows.Forms.FlowLayoutPanel mainTabFlowLayoutPanel;
		private System.Windows.Forms.Button dateTimeButton;
		private System.Windows.Forms.Button logFilesButton;
		private System.Windows.Forms.Button chartButton;
		private System.Windows.Forms.Button resetButton;
		private System.Windows.Forms.Button temperatureButton;
    }
}

