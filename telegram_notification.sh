if [ $TRAVIS_TEST_RESULT -eq 0 ]
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
Номер Build: ${TRAVIS_BUILD_NUMBER}
Log Build: ${TRAVIS_BUILD_WEB_URL}
Commit message: \"${TRAVIS_COMMIT_MESSAGE}\""