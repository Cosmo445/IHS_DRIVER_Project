
module pcihellocore (
	hexlport_external_connection_export,
	hexrport_external_connection_export,
	keysport_external_connection_export,
	lcdport_external_connection_export,
	ledsgreenport_external_connection_export,
	ledsredport_external_connection_export,
	pcie_hard_ip_0_pcie_rstn_export,
	pcie_hard_ip_0_powerdown_pll_powerdown,
	pcie_hard_ip_0_powerdown_gxb_powerdown,
	pcie_hard_ip_0_refclk_export,
	pcie_hard_ip_0_rx_in_rx_datain_0,
	pcie_hard_ip_0_tx_out_tx_dataout_0,
	swport_external_connection_export,
	gpioport_external_connection_export);	

	output	[31:0]	hexlport_external_connection_export;
	output	[31:0]	hexrport_external_connection_export;
	input	[31:0]	keysport_external_connection_export;
	output	[31:0]	lcdport_external_connection_export;
	output	[31:0]	ledsgreenport_external_connection_export;
	output	[31:0]	ledsredport_external_connection_export;
	input		pcie_hard_ip_0_pcie_rstn_export;
	input		pcie_hard_ip_0_powerdown_pll_powerdown;
	input		pcie_hard_ip_0_powerdown_gxb_powerdown;
	input		pcie_hard_ip_0_refclk_export;
	input		pcie_hard_ip_0_rx_in_rx_datain_0;
	output		pcie_hard_ip_0_tx_out_tx_dataout_0;
	input	[31:0]	swport_external_connection_export;
	output	[31:0]	gpioport_external_connection_export;
endmodule
