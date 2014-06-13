namespace Logger {
	partial class TemperatureChartTabPage {
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
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
			System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
			this.temperatureChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
			((System.ComponentModel.ISupportInitialize)(this.temperatureChart)).BeginInit();
			this.SuspendLayout();
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
			this.temperatureChart.Location = new System.Drawing.Point(3, 3);
			this.temperatureChart.Name = "temperatureChart";
			series1.ChartArea = "ChartArea1";
			series1.Name = "Series1";
			this.temperatureChart.Series.Add(series1);
			this.temperatureChart.Size = new System.Drawing.Size(637, 349);
			this.temperatureChart.TabIndex = 10;
			this.temperatureChart.Text = "chart1";
			title1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold);
			title1.Name = "Title1";
			title1.Text = "Temperature";
			this.temperatureChart.Titles.Add(title1);
			// 
			// TemperatureChartTabPage
			// 
			this.BackColor = System.Drawing.Color.White;
			this.Controls.Add(this.temperatureChart);
			this.Name = "TemperatureChartTabPage";
			this.Size = new System.Drawing.Size(640, 385);
			((System.ComponentModel.ISupportInitialize)(this.temperatureChart)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.DataVisualization.Charting.Chart temperatureChart;
	}
}
