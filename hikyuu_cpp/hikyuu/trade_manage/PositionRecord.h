/*
 * PositionRecord.h
 *
 *  Created on: 2013-2-21
 *      Author: fasiondog
 */

#pragma once
#ifndef POSITIONRECORD_H_
#define POSITIONRECORD_H_

#include "TradeRecord.h"
#include "ContractRecord.h"

#if HKU_SUPPORT_SERIALIZATION
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/list.hpp>
#endif

namespace hku {

/**
 * 持仓记录
 * @ingroup TradeManagerClass
 */
class HKU_API PositionRecord {
public:
    PositionRecord() = default;
    PositionRecord(const PositionRecord&) = default;
    PositionRecord(PositionRecord&& rv);

    PositionRecord& operator=(const PositionRecord&) = default;
    PositionRecord& operator=(PositionRecord&& rv);

    /** 仅用于python的__str__ */
    string toString() const;

    /** 根据交易记录更新仓位信息，卖出时返回需返还资金（占用保证金+盈利） */
    price_t addTradeRecord(const TradeRecord& tr);

    /**
     * 计算指定时刻前一交易日结算的持仓盈利
     * @note 按日结算
     */
    price_t getProfitOfPreDay(Datetime datetime);

    Stock stock;               ///< 交易对象
    Datetime takeDatetime;     ///< 初次建仓日期
    Datetime cleanDatetime;    ///< 平仓日期，当前持仓记录中为Null<Datetime>()
    double number = 0.0;       ///< 当前持仓数量
    price_t stoploss = 0.0;    ///< 当前止损价
    price_t goalPrice = 0.0;   ///< 当前的目标价格
    double totalNumber = 0.0;  ///< 累计持仓数量
    price_t buyMoney = 0.0;    ///< 累计买入资金
    price_t totalCost = 0.0;   ///< 累计交易总成本
    price_t totalRisk = 0.0;  ///< 累计交易风险 = 各次 （买入价格-止损)*买入数量, 不包含交易成本
    price_t sellMoney = 0.0;      ///< 累计卖出资金
    Datetime lastSettleDatetime;  ///< 上一次结算时刻
    price_t lastSettleProfit = 0.0;  ///< 已结算的盈利 （用于合约交易），即已转入可用资金的盈利
    price_t lastSettleClosePrice = 0.0;  ///< 上一次结算时的收盘价

    std::list<ContractRecord> contracts;

//===================
//序列化支持
//===================
#if HKU_SUPPORT_SERIALIZATION
private:
    friend class boost::serialization::access;
    template <class Archive>
    void save(Archive& ar, const unsigned int version) const {
        namespace bs = boost::serialization;
        ar& BOOST_SERIALIZATION_NVP(stock);
        uint64_t take = takeDatetime.number();
        uint64_t clean = cleanDatetime.number();
        ar& bs::make_nvp("takeDatetime", take);
        ar& bs::make_nvp("cleanDatetime", clean);
        ar& BOOST_SERIALIZATION_NVP(number);
        ar& BOOST_SERIALIZATION_NVP(stoploss);
        ar& BOOST_SERIALIZATION_NVP(goalPrice);
        ar& BOOST_SERIALIZATION_NVP(totalNumber);
        ar& BOOST_SERIALIZATION_NVP(buyMoney);
        ar& BOOST_SERIALIZATION_NVP(totalCost);
        ar& BOOST_SERIALIZATION_NVP(totalRisk);
        ar& BOOST_SERIALIZATION_NVP(sellMoney);
        ar& BOOST_SERIALIZATION_NVP(lastSettleDatetime);
        ar& BOOST_SERIALIZATION_NVP(lastSettleProfit);
        ar& BOOST_SERIALIZATION_NVP(lastSettleClosePrice);
        ar& BOOST_SERIALIZATION_NVP(contracts);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version) {
        namespace bs = boost::serialization;
        ar& BOOST_SERIALIZATION_NVP(stock);
        uint64_t take, clean;
        ar& bs::make_nvp("takeDatetime", take);
        ar& bs::make_nvp("cleanDatetime", clean);
        takeDatetime = Datetime(take);
        cleanDatetime = Datetime(clean);
        ar& BOOST_SERIALIZATION_NVP(number);
        ar& BOOST_SERIALIZATION_NVP(stoploss);
        ar& BOOST_SERIALIZATION_NVP(goalPrice);
        ar& BOOST_SERIALIZATION_NVP(totalNumber);
        ar& BOOST_SERIALIZATION_NVP(buyMoney);
        ar& BOOST_SERIALIZATION_NVP(totalCost);
        ar& BOOST_SERIALIZATION_NVP(totalRisk);
        ar& BOOST_SERIALIZATION_NVP(sellMoney);
        ar& BOOST_SERIALIZATION_NVP(lastSettleDatetime);
        ar& BOOST_SERIALIZATION_NVP(lastSettleProfit);
        ar& BOOST_SERIALIZATION_NVP(lastSettleClosePrice);
        ar& BOOST_SERIALIZATION_NVP(contracts);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif /* HKU_SUPPORT_SERIALIZATION */
};

/** @ingroup TradeManagerClass */
typedef vector<PositionRecord> PositionRecordList;

/**
 * 输出持仓记录信息
 * @ingroup TradeManagerClass
 */
inline std::ostream& operator<<(std::ostream& os, const PositionRecord& pos) {
    os << pos.toString();
    return os;
}

bool HKU_API operator==(const PositionRecord& d1, const PositionRecord& d2);

} /* namespace hku */
#endif /* POSITIONRECORD_H_ */
