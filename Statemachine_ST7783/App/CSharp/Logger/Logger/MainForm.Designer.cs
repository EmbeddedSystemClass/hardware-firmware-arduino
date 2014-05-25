namespace Logger {
	partial class Main {
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
			this.components = new System.ComponentModel.Container();
			Microsoft.Reporting.WinForms.ReportDataSource reportDataSource1 = new Microsoft.Reporting.WinForms.ReportDataSource();
			this.setDateTimeButton = new System.Windows.Forms.Button();
			this.portComboBox = new System.Windows.Forms.ComboBox();
			this.resetButton = new System.Windows.Forms.Button();
			this.portComboLabel = new System.Windows.Forms.Label();
			this.getTemperatureButton = new System.Windows.Forms.Button();
			this.tempLabel = new System.Windows.Forms.Label();
			this.reportViewer1 = new Microsoft.Reporting.WinForms.ReportViewer();
			this.getTemperatureLogButton = new System.Windows.Forms.Button();
			this.BindingSource = new System.Windows.Forms.BindingSource(this.components);
			this.temperatureItemBindingSource = new System.Windows.Forms.BindingSource(this.components);
			((System.ComponentModel.ISupportInitialize)(this.BindingSource)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.temperatureItemBindingSource)).BeginInit();
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
			// reportViewer1
			// 
			this.reportViewer1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			reportDataSource1.Name = "TemperatureItem";
			reportDataSource1.Value = this.BindingSource;
			this.reportViewer1.LocalReport.DataSources.Add(reportDataSource1);
			this.reportViewer1.LocalReport.ReportEmbeddedResource = "Logger.TemperatureReport.rdlc";
			this.reportViewer1.Location = new System.Drawing.Point(12, 12);
			this.reportViewer1.Name = "reportViewer1";
			this.reportViewer1.ShowToolBar = false;
			this.reportViewer1.Size = new System.Drawing.Size(543, 364);
			this.reportViewer1.TabIndex = 6;
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
			// BindingSource
			// 
			this.BindingSource.DataSource = typeof(Logger.TemperatureItem);
			// 
			// temperatureItemBindingSource
			// 
			this.temperatureItemBindingSource.DataSource = typeof(Logger.TemperatureItem);
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(663, 388);
			this.Controls.Add(this.getTemperatureLogButton);
			this.Controls.Add(this.reportViewer1);
			this.Controls.Add(this.tempLabel);
			this.Controls.Add(this.getTemperatureButton);
			this.Controls.Add(this.portComboLabel);
			this.Controls.Add(this.resetButton);
			this.Controls.Add(this.portComboBox);
			this.Controls.Add(this.setDateTimeButton);
			this.Name = "Main";
			this.Text = "Logger";
			((System.ComponentModel.ISupportInitialize)(this.BindingSource)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.temperatureItemBindingSource)).EndInit();
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
		private Microsoft.Reporting.WinForms.ReportViewer reportViewer1;
		private System.Windows.Forms.BindingSource temperatureItemBindingSource;
		private System.Windows.Forms.BindingSource BindingSource;
		private System.Windows.Forms.Button getTemperatureLogButton;		
	}
}

