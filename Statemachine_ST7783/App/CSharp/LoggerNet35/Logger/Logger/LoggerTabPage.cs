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

	public class RefreshButton : ToolStripButton {
		private ToolStripSeparator separatorButton;

		public static RefreshButton Instance = new RefreshButton();

		public void AddTo(ToolStrip toolStrip) {
			toolStrip.Items.Add(separatorButton);
			toolStrip.Items.Add(this);
		}

		public void RemoveFrom(ToolStrip toolStrip) {
			toolStrip.Items.Remove(separatorButton);
			toolStrip.Items.Remove(this);
		}

		public RefreshButton() {			
			separatorButton = new ToolStripSeparator();
			Image = ImageResource.RefreshBlue16x16;
		}
	}
}
