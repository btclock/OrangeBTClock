#include "data.hpp"

//const String mempoolInstance = "https://mempool.space";

const String mempoolPriceApi = "/api/v1/prices";
const String mempoolBlockApi = "/api/blocks/tip/height";
const String mempoolFeeApi = "/api/v1/fees/recommended";
const String mempoolMedianFeeApi = "/api/v1/fees/mempool-blocks";

uint lastPrice;
uint lastBlock;

uint getPrice()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(preferences.getString(SETTING_MEMPOOL_INSTANCE) + mempoolPriceApi);

    int httpCode = http.GET();

    // Parse JSON response and extract average price
    uint usdPrice, eurPrice;
    if (httpCode == 200)
    {
        String payload = http.getString();
        JsonDocument doc;
        deserializeJson(doc, payload);

        lastPrice = doc[preferences.getString(SETTING_CURRENCY)].as<uint>();

        return lastPrice;
    }
    else
    {
        Serial.printf("HTTP GET request price failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return 0;
}

uint getBlock()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(preferences.getString(SETTING_MEMPOOL_INSTANCE) + mempoolBlockApi);

    int httpCode = http.GET();

    // Parse JSON response and extract average price
    uint usdPrice, eurPrice;
    if (httpCode == 200)
    {
        uint payload = http.getString().toInt();

        lastBlock = payload;
        return lastBlock;
    }
    else
    {
        Serial.printf("HTTP GET request mempool block failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return 0;
}

String getMempoolFees()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(preferences.getString(SETTING_MEMPOOL_INSTANCE) + mempoolFeeApi);

    int httpCode = http.GET();

    if (httpCode == 200)
    {
        char feeString[20];
        String payload = http.getString();
        JsonDocument doc;
        deserializeJson(doc, payload);

        snprintf(feeString, 20, "L: %d M: %d H: %d", doc["hourFee"].as<uint>(), doc["halfHourFee"].as<uint>(), doc["fastestFee"].as<uint>());

        return feeString;

        // preferences.putUInt("lastPrice", eurPrice);
    }
    else
    {
        Serial.printf("HTTP GET request mempool fees failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return "";
}

uint getMempoolFeesMedian()
{
    HTTPClient http;

    // Send HTTP request to CoinGecko API
    http.begin(preferences.getString(SETTING_MEMPOOL_INSTANCE) + mempoolMedianFeeApi);

    int httpCode = http.GET();

    if (httpCode == 200)
    {
        char feeString[20];
        String payload = http.getString();
        JsonDocument doc;
        deserializeJson(doc, payload);

        snprintf(feeString, 20, "L: %d M: %d H: %d", doc["hourFee"].as<uint>(), doc["halfHourFee"].as<uint>(), doc["fastestFee"].as<uint>());

        return round(doc[0]["medianFee"].as<double>());

        // preferences.putUInt("lastPrice", eurPrice);
    }
    else
    {
        Serial.printf("HTTP GET request mempool median fees failed with error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

    return 0;
}

double getSupplyAtBlock(std::uint32_t blockNr)
{
    if (blockNr >= 33 * 210000)
    {
        return 20999999.9769;
    }

    const int initialBlockReward = 50;  // Initial block reward
    const int halvingInterval = 210000; // Number of blocks before halving

    int halvingCount = blockNr / halvingInterval;
    double totalBitcoinInCirculation = 0;

    for (int i = 0; i < halvingCount; ++i)
    {
        totalBitcoinInCirculation += halvingInterval * initialBlockReward * std::pow(0.5, i);
    }

    totalBitcoinInCirculation += (blockNr % halvingInterval) * initialBlockReward * std::pow(0.5, halvingCount);

    return totalBitcoinInCirculation;
}

String formatNumberWithSuffix(std::uint64_t num, int numCharacters)
{
    static char result[20]; // Adjust size as needed
    const long long quadrillion = 1000000000000000LL;
    const long long trillion = 1000000000000LL;
    const long long billion = 1000000000;
    const long long million = 1000000;
    const long long thousand = 1000;

    double numDouble = (double)num;
    int numDigits = (int)log10(num) + 1;
    char suffix;

    if (num >= quadrillion || numDigits > 15)
    {
        numDouble /= quadrillion;
        suffix = 'Q';
    }
    else if (num >= trillion || numDigits > 12)
    {
        numDouble /= trillion;
        suffix = 'T';
    }
    else if (num >= billion || numDigits > 9)
    {
        numDouble /= billion;
        suffix = 'B';
    }
    else if (num >= million || numDigits > 6)
    {
        numDouble /= million;
        suffix = 'M';
    }
    else if (num >= thousand || numDigits > 3)
    {
        numDouble /= thousand;
        suffix = 'K';
    }
    else
    {
        sprintf(result, "%llu", (unsigned long long)num);
        return result;
    }

    // Add suffix
    int len = snprintf(result, sizeof(result), "%.0f%c", numDouble, suffix);

    // If there's room, add decimal places
    if (len < numCharacters)
    {
        snprintf(result, sizeof(result), "%.*f%c", numCharacters - len - 1, numDouble, suffix);
    }

    return result;
}