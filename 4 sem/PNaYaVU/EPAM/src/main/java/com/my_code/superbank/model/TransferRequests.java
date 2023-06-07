package com.my_code.superbank.model;

import lombok.Data;
import java.math.BigDecimal;
import java.util.List;

@Data
public class TransferRequests { // для добавления
    private Long to;
    private List<BigDecimal> amount;
}