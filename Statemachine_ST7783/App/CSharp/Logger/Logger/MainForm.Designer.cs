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
			this.setDateTimeButton = new System.Windows.Forms.Button();
			this.portComboBox = new System.Windows.Forms.ComboBox();
			this.resetButton = new System.Windows.Forms.Button();
			this.portComboLabel = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// setDateTimeButton
			// 
			this.setDateTimeButton.Location = new System.Drawing.Point(79, 79);
			this.setDateTimeButton.Name = "setDateTimeButton";
			this.setDateTimeButton.Size = new System.Drawing.Size(127, 23);
			this.setDateTimeButton.TabIndex = 1;
			this.setDateTimeButton.Text = "Set DateTime";
			this.setDateTimeButton.UseVisualStyleBackColor = true;
			this.setDateTimeButton.Click += new System.EventHandler(this.setDateTimeButton_Click);
			// 
			// portComboBox
			// 
			this.portComboBox.FormattingEnabled = true;
			this.portComboBox.Location = new System.Drawing.Point(122, 28);
			this.portComboBox.Name = "portComboBox";
			this.portComboBox.Size = new System.Drawing.Size(84, 21);
			this.portComboBox.TabIndex = 0;
			// 
			// resetButton
			// 
			this.resetButton.Location = new System.Drawing.Point(79, 108);
			this.resetButton.Name = "resetButton";
			this.resetButton.Size = new System.Drawing.Size(127, 23);
			this.resetButton.TabIndex = 2;
			this.resetButton.Text = "Reset";
			this.resetButton.UseVisualStyleBackColor = true;
			this.resetButton.Click += new System.EventHandler(this.resetButton_Click);
			// 
			// portComboLabel
			// 
			this.portComboLabel.AutoSize = true;
			this.portComboLabel.Location = new System.Drawing.Point(76, 31);
			this.portComboLabel.Name = "portComboLabel";
			this.portComboLabel.Size = new System.Drawing.Size(29, 13);
			this.portComboLabel.TabIndex = 3;
			this.portComboLabel.Text = "Port:";
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 143);
			this.Controls.Add(this.portComboLabel);
			this.Controls.Add(this.resetButton);
			this.Controls.Add(this.portComboBox);
			this.Controls.Add(this.setDateTimeButton);
			this.Name = "Main";
			this.Text = "Logger";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button setDateTimeButton;
		private System.Windows.Forms.ComboBox portComboBox;
		private System.Windows.Forms.Button resetButton;
		private System.Windows.Forms.Label portComboLabel;
	}
}

