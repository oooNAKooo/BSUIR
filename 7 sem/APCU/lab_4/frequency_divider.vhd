library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity clk_div is
    Port ( clk_in: in std_logic;
           clk_out: out std_logic);
end clk_div;

architecture Behavioral of clk_div is
    signal temp_clk : std_logic := '0';
    
    constant divide_value: integer := 200000000 / 2; -- Делитель частоты
begin
    process (clk_in)
       variable i  : integer := 0;
    begin
        if clk_in'event and clk_in = '1' then
            if (i = divide_value) then
                i := 0;
                temp_clk <= not temp_clk;
            else 
                i := i + 1;
            end if;
        end if;
    end process;
    
    clk_out <= temp_clk;       
end Behavioral;
