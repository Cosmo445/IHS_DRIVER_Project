	component pcihellocore is
		port (
			hexlport_external_connection_export      : out std_logic_vector(31 downto 0);                    -- export
			hexrport_external_connection_export      : out std_logic_vector(31 downto 0);                    -- export
			keysport_external_connection_export      : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			lcdport_external_connection_export       : out std_logic_vector(31 downto 0);                    -- export
			ledsgreenport_external_connection_export : out std_logic_vector(31 downto 0);                    -- export
			ledsredport_external_connection_export   : out std_logic_vector(31 downto 0);                    -- export
			pcie_hard_ip_0_pcie_rstn_export          : in  std_logic                     := 'X';             -- export
			pcie_hard_ip_0_powerdown_pll_powerdown   : in  std_logic                     := 'X';             -- pll_powerdown
			pcie_hard_ip_0_powerdown_gxb_powerdown   : in  std_logic                     := 'X';             -- gxb_powerdown
			pcie_hard_ip_0_refclk_export             : in  std_logic                     := 'X';             -- export
			pcie_hard_ip_0_rx_in_rx_datain_0         : in  std_logic                     := 'X';             -- rx_datain_0
			pcie_hard_ip_0_tx_out_tx_dataout_0       : out std_logic;                                        -- tx_dataout_0
			swport_external_connection_export        : in  std_logic_vector(31 downto 0) := (others => 'X'); -- export
			gpioport_external_connection_export      : out std_logic_vector(31 downto 0)                     -- export
		);
	end component pcihellocore;

	u0 : component pcihellocore
		port map (
			hexlport_external_connection_export      => CONNECTED_TO_hexlport_external_connection_export,      --      hexlport_external_connection.export
			hexrport_external_connection_export      => CONNECTED_TO_hexrport_external_connection_export,      --      hexrport_external_connection.export
			keysport_external_connection_export      => CONNECTED_TO_keysport_external_connection_export,      --      keysport_external_connection.export
			lcdport_external_connection_export       => CONNECTED_TO_lcdport_external_connection_export,       --       lcdport_external_connection.export
			ledsgreenport_external_connection_export => CONNECTED_TO_ledsgreenport_external_connection_export, -- ledsgreenport_external_connection.export
			ledsredport_external_connection_export   => CONNECTED_TO_ledsredport_external_connection_export,   --   ledsredport_external_connection.export
			pcie_hard_ip_0_pcie_rstn_export          => CONNECTED_TO_pcie_hard_ip_0_pcie_rstn_export,          --          pcie_hard_ip_0_pcie_rstn.export
			pcie_hard_ip_0_powerdown_pll_powerdown   => CONNECTED_TO_pcie_hard_ip_0_powerdown_pll_powerdown,   --          pcie_hard_ip_0_powerdown.pll_powerdown
			pcie_hard_ip_0_powerdown_gxb_powerdown   => CONNECTED_TO_pcie_hard_ip_0_powerdown_gxb_powerdown,   --                                  .gxb_powerdown
			pcie_hard_ip_0_refclk_export             => CONNECTED_TO_pcie_hard_ip_0_refclk_export,             --             pcie_hard_ip_0_refclk.export
			pcie_hard_ip_0_rx_in_rx_datain_0         => CONNECTED_TO_pcie_hard_ip_0_rx_in_rx_datain_0,         --              pcie_hard_ip_0_rx_in.rx_datain_0
			pcie_hard_ip_0_tx_out_tx_dataout_0       => CONNECTED_TO_pcie_hard_ip_0_tx_out_tx_dataout_0,       --             pcie_hard_ip_0_tx_out.tx_dataout_0
			swport_external_connection_export        => CONNECTED_TO_swport_external_connection_export,        --        swport_external_connection.export
			gpioport_external_connection_export      => CONNECTED_TO_gpioport_external_connection_export       --      gpioport_external_connection.export
		);

