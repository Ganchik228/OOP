namespace Lab1
{
    
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            this.Text = textBox1.Text;
        }

        private void Form1_DoubleClick(object sender, EventArgs e)
        {
            textBox1.Text = string.Empty;
        }
    }
}
