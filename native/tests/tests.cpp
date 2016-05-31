/******************************************************************************
Created by Adam Streck, 2013-2015, adam.streck@fu-berlin.de

This file is part of the Toolkit for Reverse Engineering of Molecular Pathways
via Parameter Identification (TREMPPI)

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "tests.hpp"

TEST(CoreTest, AllPrograms)
{
	bfs::remove_all(TremppiSystem::HOME_PATH / TEST_FOLDER / TEST_PROJECT);
	ASSERT_EQ(0, basic_init_test()) << "Basic content test initiation failed.";
	ASSERT_EQ(0, basic_spawn_test()) << "Basic content test spawn failed.";
	ASSERT_EQ(0, basic_express_test()) << "Basic content test express failed.";
	ASSERT_EQ(0, basic_sign_test()) << "Basic content test label failed.";
	ASSERT_EQ(0, basic_bias_test()) << "Basic content test bias failed.";
	ASSERT_EQ(0, basic_impact_test()) << "Basic content test impact failed.";
	ASSERT_EQ(0, basic_validate_test()) << "Basic content test validate failed.";
	ASSERT_EQ(0, basic_regulations_test()) << "Basic content test regulations failed.";
	ASSERT_EQ(0, basic_correlations_test()) << "Basic content test correlations test failed.";
	ASSERT_EQ(0, basic_witness_test()) << "Basic content test witness failed.";
	ASSERT_EQ(0, basic_qualitative_test()) << "Basic content test qualitative failed.";
	ASSERT_EQ(0, basic_quantitative_test()) << "Basic content test quantitative failed.";
	ASSERT_EQ(0, basic_group_test()) << "Basic content test list failed.";
	ASSERT_EQ(0, basic_update_test()) << "Basic content test update failed.";
	cout << endl;
}

int tremppi_test(int argc, char** argv)
{
	TremppiSystem::initiate("tremppi_test", argc, argv);

	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	return result;
}
