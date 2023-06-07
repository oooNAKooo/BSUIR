package com.my_code.superbank.model;

import lombok.AllArgsConstructor;
import lombok.Data;
import java.math.BigDecimal;
@Data
@AllArgsConstructor
public class BalanceResponse {
    private BigDecimal balance;
}