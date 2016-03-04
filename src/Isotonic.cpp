
#include "Isotonic.hpp"
#include "pmml-4-2.hpp"

namespace mPMML
{
    double LinearNorm::operator()(double x) const {
        auto rMargin = std::lower_bound(orig.begin(), orig.end(), x);
        if (rMargin == orig.begin()) {
            rMargin++;
        } else if (rMargin == orig.end()) {
            rMargin--;
        }
        const size_t idx = rMargin - orig.begin();
        double b1 = norm[idx-1];
        double b2 = norm[idx];
        double a1 = orig[idx-1];
        double a2 = orig[idx];
        double res = b1+(x-a1)/(a2-a1)*(b2-b1);
        return res;
    }

    Transformations read_linear_norm(const std::string& fname, const std::string& name) 
    {
        xml_schema::properties props;
        props.schema_location ("http://www.dmg.org/PMML-4_2", "pmml-4-2.xsd");
        auto doc = PMML::PMML_(fname, 0, props);

        const auto dd = doc->DataDictionary();
        const auto dfs = dd.DataField();
        if (dfs[0].name() != "budgetRate") {
            throw Exception("wrong input variable");
        }
        if (dfs[1].name() != "minCTR") {
            throw Exception("wrong output variable");
        }

        if (doc->RegressionModel().size() != 1) {
            throw Exception("ony one RegressionModel supported");
        }
        const auto& rmodel = doc->RegressionModel()[0];
        const auto& mschema = rmodel.MiningSchema();
        const auto& mf = mschema.MiningField();
        if (mf[0].name() != "budgetRate") {
            throw Exception("wrong input variable");
        }
        if (mf[1].name() != "minCTR") {
            throw Exception("wrong output variable");
        }

        const auto& ltrans = rmodel.LocalTransformations(); // optional field, `->` to use
        if (!ltrans) {
            throw Exception("LocalTransformations not found");
        }
        if (ltrans->DerivedField().size() != 1) {
            throw Exception("only one DerivedField supported");
        }

        const auto& df = ltrans->DerivedField()[0];
        if (df.name().get() != name) {
            throw Exception("unexpected DerivedField name");
        }
        if (df.optype() != "continuous") {
            throw Exception("unexpected DerivedField optype");
        }

        const auto& nc = df.NormContinuous();               // optional
        if (!nc) {
            throw Exception("NormContinuous not found");
        }
        if (nc->field() != "budgetRate") {
            throw Exception("unexpected NormContinuous field");
        }
        if (nc->outliers() != "asIs") {
            throw Exception("unexpected NormContinuous outlier");
        }

        const auto& vList = nc->LinearNorm();
        if (vList.size() <= 1) {
            throw Exception("LinearNorm list must contain at least 2 elements");
        }

        if (rmodel.RegressionTable().size() != 1) {
            throw Exception("only one RegressionTable supported");
        }
        const auto& rt = rmodel.RegressionTable()[0];
        const auto& np = rt.NumericPredictor();
        Regression reg;
        for (auto x = np.begin(); x != np.end(); x++) {
            if (x->name() == name) {
                reg = Regression(rt, *x);
                break;
            }
        }

        Transformations tran(vList, reg);
        return tran;
    }
}

