package com.my_code.superbank.model;

import lombok.Data;
import java.math.BigDecimal;
// чтобы не прописывать геттеры и сеттеры прописываем анатацию дату
// @Data - генерит конструктор, геттер, сеттер, иквэл и хэшкод
@Data
public class transferBalance {
    private Long from;
    private Long to;
    private BigDecimal amount;
}