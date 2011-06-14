/*
 * Plan Instrment for PostgreSQL 9.0, 9.1
 *
 * 2011, Hitoshi Harada
 */
#include "postgres.h"
#include "fmgr.h"

#include "commands/explain.h"
#include "optimizer/planner.h"
#include "portability/instr_time.h"
#include "tcop/tcopprot.h"

PG_MODULE_MAGIC;

static planner_hook_type prev_planner = NULL;
static ExplainOneQuery_hook_type prev_ExplainOneQuery = NULL;

void _PG_init(void);
void _PG_fini(void);

static PlannedStmt *planinstr_planner(Query *parse, int cursorOptions,
				ParamListInfo boundParams);
static void planinstr_ExplainOneQuery(Query *query, ExplainState *es,
				const char *queryString, ParamListInfo params);
static double gettime(void);

static double plan_duration = 0.0;
static bool instr_enabled = true;

void
_PG_init(void)
{
	/* Install hookds. */
	prev_ExplainOneQuery = ExplainOneQuery_hook;
	ExplainOneQuery_hook = planinstr_ExplainOneQuery;
	prev_planner = planner_hook;
	planner_hook = planinstr_planner;
}

void
_PG_fini(void)
{
	/* Uninstall hooks. */
	planner_hook = prev_planner;
	ExplainOneQuery_hook = prev_ExplainOneQuery;
}

static void
planinstr_ExplainOneQuery(Query *query, ExplainState *es,
						  const char *queryString, ParamListInfo params)
{
	if (prev_ExplainOneQuery)
		prev_ExplainOneQuery(query, es, queryString, params);
	else
	{
		PlannedStmt *plan;

		plan = pg_plan_query(query, 0, params);
		ExplainOnePlan(plan, es, queryString, params);
	}

	if (instr_enabled)
	{
		if (es->format == EXPLAIN_FORMAT_TEXT)
		{
			char	buf[256];

			snprintf(buf, sizeof(buf), "%.3f ms", plan_duration);
			ExplainPropertyText("Plan time", buf, es);
		}
		else
			ExplainPropertyFloat("Plan time", plan_duration, 3, es);
	}
}

static PlannedStmt *
planinstr_planner(Query *parse, int cursorOptions,
				  ParamListInfo boundParams)
{
	double	t0;
	PlannedStmt *plan;

	if (instr_enabled)
	{
		t0 = gettime();
	}

	if (prev_planner)
		plan = prev_planner(parse, cursorOptions, boundParams);
	else
		plan = standard_planner(parse, cursorOptions, boundParams);

	if (instr_enabled)
	{
		plan_duration = gettime() - t0;
	}

	return plan;
}

static double
gettime(void)
{
	instr_time t;

	INSTR_TIME_SET_CURRENT(t);
	return INSTR_TIME_GET_MILLISEC(t);
}
