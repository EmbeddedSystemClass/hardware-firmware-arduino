namespace Logger {
	partial class DateTimeTabPage {
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
            this.components = new System.ComponentModel.Container();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.setSystemTime = new System.Windows.Forms.Button();
            this.setSystemDate = new System.Windows.Forms.Button();
            this.systemTimeLabel = new System.Windows.Forms.Label();
            this.systemDateLabel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.setTimeButton = new System.Windows.Forms.Button();
            this.setDateButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.datePicker = new System.Windows.Forms.DateTimePicker();
            this.label2 = new System.Windows.Forms.Label();
            this.timePicker = new System.Windows.Forms.DateTimePicker();
            this.updateTimer = new System.Windows.Forms.Timer(this.components);
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.setSystemTime);
            this.groupBox2.Controls.Add(this.setSystemDate);
            this.groupBox2.Controls.Add(this.systemTimeLabel);
            this.groupBox2.Controls.Add(this.systemDateLabel);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Location = new System.Drawing.Point(3, 120);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(327, 109);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            // 
            // setSystemTime
            // 
            this.setSystemTime.Location = new System.Drawing.Point(268, 70);
            this.setSystemTime.Name = "setSystemTime";
            this.setSystemTime.Size = new System.Drawing.Size(47, 20);
            this.setSystemTime.TabIndex = 14;
            this.setSystemTime.Text = "Set";
            this.setSystemTime.UseVisualStyleBackColor = true;
            this.setSystemTime.Visible = false;
            this.setSystemTime.Click += new System.EventHandler(this.setSystemTimeButton_Click);
            // 
            // setSystemDate
            // 
            this.setSystemDate.Location = new System.Drawing.Point(268, 32);
            this.setSystemDate.Name = "setSystemDate";
            this.setSystemDate.Size = new System.Drawing.Size(47, 20);
            this.setSystemDate.TabIndex = 13;
            this.setSystemDate.Text = "Set";
            this.setSystemDate.UseVisualStyleBackColor = true;
            this.setSystemDate.Click += new System.EventHandler(this.setSystemDateButton_Click);
            // 
            // systemTimeLabel
            // 
            this.systemTimeLabel.BackColor = System.Drawing.SystemColors.Control;
            this.systemTimeLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.systemTimeLabel.Location = new System.Drawing.Point(9, 70);
            this.systemTimeLabel.Name = "systemTimeLabel";
            this.systemTimeLabel.Size = new System.Drawing.Size(86, 20);
            this.systemTimeLabel.TabIndex = 12;
            this.systemTimeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // systemDateLabel
            // 
            this.systemDateLabel.BackColor = System.Drawing.SystemColors.Control;
            this.systemDateLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.systemDateLabel.Location = new System.Drawing.Point(9, 32);
            this.systemDateLabel.Name = "systemDateLabel";
            this.systemDateLabel.Size = new System.Drawing.Size(200, 20);
            this.systemDateLabel.TabIndex = 11;
            this.systemDateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 16);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(33, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Date:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 55);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(33, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Time:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.setTimeButton);
            this.groupBox1.Controls.Add(this.setDateButton);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.datePicker);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.timePicker);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(327, 111);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            // 
            // setTimeButton
            // 
            this.setTimeButton.Location = new System.Drawing.Point(268, 71);
            this.setTimeButton.Name = "setTimeButton";
            this.setTimeButton.Size = new System.Drawing.Size(47, 20);
            this.setTimeButton.TabIndex = 7;
            this.setTimeButton.Text = "Set";
            this.setTimeButton.UseVisualStyleBackColor = true;
            this.setTimeButton.Visible = false;
            this.setTimeButton.Click += new System.EventHandler(this.setTimeButton_Click);
            // 
            // setDateButton
            // 
            this.setDateButton.Location = new System.Drawing.Point(268, 32);
            this.setDateButton.Name = "setDateButton";
            this.setDateButton.Size = new System.Drawing.Size(47, 20);
            this.setDateButton.TabIndex = 6;
            this.setDateButton.Text = "Set";
            this.setDateButton.UseVisualStyleBackColor = true;
            this.setDateButton.Click += new System.EventHandler(this.setDateButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Date:";
            // 
            // datePicker
            // 
            this.datePicker.Location = new System.Drawing.Point(9, 32);
            this.datePicker.Name = "datePicker";
            this.datePicker.Size = new System.Drawing.Size(200, 20);
            this.datePicker.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Time:";
            // 
            // timePicker
            // 
            this.timePicker.CustomFormat = "";
            this.timePicker.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.timePicker.Location = new System.Drawing.Point(9, 71);
            this.timePicker.Name = "timePicker";
            this.timePicker.ShowUpDown = true;
            this.timePicker.Size = new System.Drawing.Size(86, 20);
            this.timePicker.TabIndex = 1;
            // 
            // updateTimer
            // 
            this.updateTimer.Enabled = true;
            this.updateTimer.Interval = 500;
            this.updateTimer.Tick += new System.EventHandler(this.updateTimer_Tick);
            // 
            // DateTimeTabPage
            // 
            this.BackColor = System.Drawing.Color.White;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "DateTimeTabPage";
            this.Size = new System.Drawing.Size(491, 315);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button setSystemDate;
		private System.Windows.Forms.Label systemTimeLabel;
		private System.Windows.Forms.Label systemDateLabel;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button setDateButton;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.DateTimePicker datePicker;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.DateTimePicker timePicker;
		private System.Windows.Forms.Timer updateTimer;
        private System.Windows.Forms.Button setTimeButton;
        private System.Windows.Forms.Button setSystemTime;
	}
}
