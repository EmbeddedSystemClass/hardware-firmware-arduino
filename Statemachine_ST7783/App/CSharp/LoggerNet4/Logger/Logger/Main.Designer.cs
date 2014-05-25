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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
            this.setDateTimeButton = new System.Windows.Forms.Button();
            this.portComboBox = new System.Windows.Forms.ComboBox();
            this.resetButton = new System.Windows.Forms.Button();
            this.portComboLabel = new System.Windows.Forms.Label();
            this.getTemperatureButton = new System.Windows.Forms.Button();
            this.tempLabel = new System.Windows.Forms.Label();
            this.getTemperatureLogButton = new System.Windows.Forms.Button();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.SuspendLayout();
            // 
            // setDateTimeButton
            // 
            this.setDateTimeButton.Location = new System.Drawing.Point(561, 39);
            this.setDateTimeButton.Name = "setDateTimeButton";
            this.setDateTimeButton.Size = new System.Drawing.Size(95, 23);
            this.setDateTimeButton.TabIndex = 1;
            this.setDateTimeButton.Text = "Set DateTime";
            this.setDateTimeButton.UseVisualStyleBackColor = true;
            this.setDateTimeButton.Click += new System.EventHandler(this.setDateTimeButton_Click);
            // 
            // portComboBox
            // 
            this.portComboBox.FormattingEnabled = true;
            this.portComboBox.Location = new System.Drawing.Point(593, 12);
            this.portComboBox.Name = "portComboBox";
            this.portComboBox.Size = new System.Drawing.Size(63, 21);
            this.portComboBox.TabIndex = 0;
            this.portComboBox.SelectedIndexChanged += new System.EventHandler(this.portComboBox_SelectedIndexChanged);
            // 
            // resetButton
            // 
            this.resetButton.Location = new System.Drawing.Point(561, 149);
            this.resetButton.Name = "resetButton";
            this.resetButton.Size = new System.Drawing.Size(95, 23);
            this.resetButton.TabIndex = 2;
            this.resetButton.Text = "Reset";
            this.resetButton.UseVisualStyleBackColor = true;
            this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
            // 
            // portComboLabel
            // 
            this.portComboLabel.AutoSize = true;
            this.portComboLabel.Location = new System.Drawing.Point(558, 15);
            this.portComboLabel.Name = "portComboLabel";
            this.portComboLabel.Size = new System.Drawing.Size(29, 13);
            this.portComboLabel.TabIndex = 3;
            this.portComboLabel.Text = "Port:";
            // 
            // getTemperatureButton
            // 
            this.getTemperatureButton.Location = new System.Drawing.Point(561, 97);
            this.getTemperatureButton.Name = "getTemperatureButton";
            this.getTemperatureButton.Size = new System.Drawing.Size(95, 23);
            this.getTemperatureButton.TabIndex = 4;
            this.getTemperatureButton.Text = "Get Temp";
            this.getTemperatureButton.UseVisualStyleBackColor = true;
            this.getTemperatureButton.Click += new System.EventHandler(this.getTemperatureButton_Click);
            // 
            // tempLabel
            // 
            this.tempLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.tempLabel.Location = new System.Drawing.Point(561, 123);
            this.tempLabel.Name = "tempLabel";
            this.tempLabel.Size = new System.Drawing.Size(95, 23);
            this.tempLabel.TabIndex = 5;
            this.tempLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // getTemperatureLogButton
            // 
            this.getTemperatureLogButton.Location = new System.Drawing.Point(561, 68);
            this.getTemperatureLogButton.Name = "getTemperatureLogButton";
            this.getTemperatureLogButton.Size = new System.Drawing.Size(95, 23);
            this.getTemperatureLogButton.TabIndex = 7;
            this.getTemperatureLogButton.Text = "Get Log";
            this.getTemperatureLogButton.UseVisualStyleBackColor = true;
            this.getTemperatureLogButton.Click += new System.EventHandler(this.getTemperatureLogButton_Click);
            // 
            // chart1
            // 
            this.chart1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.chart1.BorderlineColor = System.Drawing.Color.Black;
            this.chart1.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            chartArea1.AxisX.MajorGrid.Enabled = false;
            chartArea1.AxisX.Title = "Hours";
            chartArea1.AxisY.TextOrientation = System.Windows.Forms.DataVisualization.Charting.TextOrientation.Horizontal;
            chartArea1.AxisY.Title = "°C";
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            this.chart1.Location = new System.Drawing.Point(12, 12);
            this.chart1.Name = "chart1";
            series1.ChartArea = "ChartArea1";
            series1.Name = "Series1";
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(524, 375);
            this.chart1.TabIndex = 8;
            this.chart1.Text = "chart1";
            title1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            title1.Name = "Title1";
            title1.Text = "Temperature";
            this.chart1.Titles.Add(title1);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(668, 399);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.getTemperatureLogButton);
            this.Controls.Add(this.tempLabel);
            this.Controls.Add(this.getTemperatureButton);
            this.Controls.Add(this.portComboLabel);
            this.Controls.Add(this.resetButton);
            this.Controls.Add(this.portComboBox);
            this.Controls.Add(this.setDateTimeButton);
            this.Name = "Main";
            this.Text = "Logger";
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button setDateTimeButton;
        private System.Windows.Forms.ComboBox portComboBox;
        private System.Windows.Forms.Button resetButton;
        private System.Windows.Forms.Label portComboLabel;
        private System.Windows.Forms.Button getTemperatureButton;
        private System.Windows.Forms.Label tempLabel;
        private System.Windows.Forms.Button getTemperatureLogButton;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
    }
}

