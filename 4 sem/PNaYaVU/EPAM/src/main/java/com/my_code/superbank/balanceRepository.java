package com.my_code.superbank;

import org.springframework.stereotype.Repository;
import java.math.BigDecimal;
import java.util.HashMap;
import java.util.Map;

// хранилище в виде хэшмапа
@Repository
public class balanceRepository {

    // long - ключ - аккаунт пользователя
    // BigDecimal - баланс счета
    // в данном случае, если у нас наш ключ = 1, то выведет счет = 10, в иных - ошибка
    private final Map<Long, BigDecimal> storage = new HashMap<>(Map.of(1L, BigDecimal.ZERO)); // начальные бабки на первом акке

    public BigDecimal getBalanceForID(Long accountID){
        return storage.get(accountID);
    }

    public void save(Long id, BigDecimal amount) {
        storage.put(id, amount);
    }
}