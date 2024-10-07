library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Multiplexer16x1 is
    Port ( A : in STD_LOGIC_VECTOR (3 downto 0);  -- Входы выбора
           D : out STD_LOGIC_VECTOR (15 downto 0); -- Входы данных
           Y : out STD_LOGIC);                    -- Выход
end Multiplexer16x1;

architecture Behavioral of Multiplexer16x1 is
begin
    process(A)
    begin
        case A is
            when "0000" => D <= "0000000000000001";  -- E0
            Y <= '0';
            when "0001" => D <= "0000000000000010";  -- E1
            Y <= '1';
            when "0010" => D <= "0000000000000100";  -- E2
            Y <= '1';
            when "0011" => D <= "0000000000001000";  -- E3
            Y <= '1';
            when "0100" => D <= "0000000000010000";  -- E4
            Y <= '1';
            when "0101" => D <= "0000000000100000";  -- E5
            Y <= '1';
            when "0110" => D <= "0000000001000000";  -- E6
            Y <= '1';
            when "0111" => D <= "0000000010000000";  -- E7
            Y <= '1';
            when "1000" => D <= "0000000100000000";  -- E8
            Y <= '1';
            when "1001" => D <= "0000001000000000";  -- E9
            Y <= '1';
            when "1010" => D <= "0000010000000000"; -- E10
            Y <= '1';
            when "1011" => D <= "0000100000000000"; -- E11
            Y <= '1';
            when "1100" => D <= "0001000000000000"; -- E12
            Y <= '1';
            when "1101" => D <= "0010000000000000"; -- E13
            Y <= '1';
            when "1110" => D <= "0100000000000000"; -- E14
            Y <= '1';
            when "1111" => D <= "1000000000000000"; -- E15
            Y <= '1';
            when others => D <= "0000000000000000";
        end case;
    end process;
end Behavioral;
