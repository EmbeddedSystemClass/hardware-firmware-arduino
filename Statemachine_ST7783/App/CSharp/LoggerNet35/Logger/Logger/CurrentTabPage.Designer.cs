namespace Logger {
	partial class CurrentTabPage {
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.sensorView = new System.Windows.Forms.ListView();
			this.Sensor = new System.Windows.Forms.ColumnHeader();
			this.Value = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// sensorView
			// 
			this.sensorView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Sensor,
            this.Value});
			this.sensorView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.sensorView.Location = new System.Drawing.Point(0, 0);
			this.sensorView.Name = "sensorView";
			this.sensorView.Size = new System.Drawing.Size(525, 311);
			this.sensorView.TabIndex = 4;
			this.sensorView.UseCompatibleStateImageBehavior = false;
			this.sensorView.View = System.Windows.Forms.View.Details;
			// 
			// Sensor
			// 
			this.Sensor.Text = "Sensor";
			this.Sensor.Width = 81;
			// 
			// Value
			// 
			this.Value.Text = "Value";
			this.Value.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.Value.Width = 133;
			// 
			// TemperatureTabPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.White;
			this.Controls.Add(this.sensorView);
			this.Name = "TemperatureTabPage";
			this.Size = new System.Drawing.Size(525, 311);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ListView sensorView;
		private System.Windows.Forms.ColumnHeader Sensor;
		private System.Windows.Forms.ColumnHeader Value;

	}
}
