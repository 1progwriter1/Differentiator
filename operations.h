DEF_OP (ADD, 0, "+", 2,
{
    NEW(OP(ADD), D(L), D(R))
},
{
    CALC(L) + CALC(R)
})

DEF_OP (SUB, 1, "-", 2,
        NEW(OP(SUB), D(L), D(R)),
        CALC(L) - CALC(R))

DEF_OP (MUL, 2, "*", 2,
        NEW(OP(ADD), NEW(OP(MUL), D(L), C(R)), NEW(OP(MUL), C(L), D(R))),
        CALC(L) * CALC(R)) //

DEF_OP (DIV, 3, "/", 2,
        NEW(OP(DIV), NEW(OP(SUB), NEW(OP(MUL), D(L), C(R)), NEW(OP(MUL), C(L), D(R))), NEW(OP(POW), C(R), NEW(NUM(2), NULL, NULL))),
        CALC(L) / CALC(R))

DEF_OP (SIN, 4, "sin", 1,
        NEW(OP(MUL), NEW(OP(COS), NULL, C(R)), D(R)),
        sin(CALC(R) * PI / 180))

DEF_OP (COS, 5, "cos", 1,
        NEW(OP(MUL), NEW(NUM(-1), NULL, NULL), NEW(OP(MUL), NEW(OP(SIN), NULL, C(R)), D(R))),
        cos(CALC(R) * PI / 180))

DEF_OP (LN, 6, "ln", 1,
        NEW(OP(MUL), NEW(OP(DIV), NEW(NUM(1), NULL, NULL), C(R)), D(R)),
        log(CALC(R)))

DEF_OP (POW, 7, "^", 2,
        NEW(NUM(100), NULL, NULL),
        pow(CALC(L), CALC(R)))

DEF_OP (SQRT, 8, "sqrt", 1,
        NEW(NUM(200), NULL, NULL),
        sqrt(CALC(R)))