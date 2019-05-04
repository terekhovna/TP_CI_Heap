if [ $TRAVIS_TEST_RESULT -eq 1 ]
then
    built_r="successed"
else
    built_r="failed"
fi

curl -s -X POST\
    "https://api.telegram.org/bot${TELEGRAM_TOKEN}/sendMessage"\
    -F chat_id="${TELEGRAM_CHAT_ID}"\
    -F text=\
"Результат сборки: ${built_r}
Номер Job:${TRAVIS_JOB_NUMBER}
Log Job:${TRAVIS_JOB_WEB_URL}
Log Build:${TRAVIS_BUILD_WEB_URL}
Commit:${TRAVIS_COMMIT}"