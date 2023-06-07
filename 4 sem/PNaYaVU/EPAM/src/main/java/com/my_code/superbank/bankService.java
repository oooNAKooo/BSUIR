package com.my_code.superbank;

import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;
import java.math.BigDecimal;

// данный класс для взаимодействия с переводом денег
@Service
@AllArgsConstructor
public class bankService {

    private final balanceRepository repository;
    public BigDecimal getBalance(Long accountID){
        BigDecimal balance = repository.getBalanceForID(accountID);
        if(balance == null){
            throw new IllegalArgumentException();
        }
        return balance;
    }
    public BigDecimal addMoney(Long to, BigDecimal amount) {
        BigDecimal currentBalance = repository.getBalanceForID(to);
        if (currentBalance == null) {
            repository.save(to, amount);
            return amount;
        } else {
            BigDecimal updatedBalance = currentBalance.add(amount);
            repository.save(to, updatedBalance);
            return updatedBalance;
        }
    }
    public BigDecimal divideBalance(Long accountID) {
        BigDecimal balance = repository.getBalanceForID(accountID);
        if (balance == null) {
            throw new IllegalArgumentException("Invalid account ID");
        }
        return balance.divide(BigDecimal.valueOf(4));
    }
}