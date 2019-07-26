#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "../builtins/type.hpp"
#include "../ast/node.hpp"
#include "../ir_generator/ir_generator.hpp"
#include "ast_generator.hpp"


ASTGenerator::~ASTGenerator() {}

ASTGenerator::ASTGenerator(llvm::Module& module,llvm::IRBuilder<>& ir_builder,llvm::LLVMContext& context)
:module(module),ir_builder(ir_builder),context(context),current_namespace({"__TOP"})
{
    ir_generator = std::make_shared<IRGenerator>(ir_builder,context);
    int_ir_generator = std::make_shared<IntergerIRGenerator>(ir_builder,context);
    float_ir_generator = std::make_shared<FloatIRGenerator>(ir_builder,context);
    binary_expression_generator = std::make_shared<BinaryExpressionIRGenerator>(ir_builder,context);
}

void ASTGenerator::generate()
{/*
    for (int i=0;i<nodes.size();i++)
    {
        //nodes[i]->generate();
    }
*/
}

void ASTGenerator::into_namespace(std::string name)
{
    current_namespace.push_back(name);
    std::map<std::string,std::shared_ptr<Node>> vals;
    variables[current_namespace] = vals;
}

void ASTGenerator::break_out_of_namespace()
{
    if (!current_namespace.empty())
    {
    current_namespace.pop_back();
    }
}

std::shared_ptr<Node> ASTGenerator::add_variable(std::string name,std::shared_ptr<Node> right_value)
{
    std::string cur = ".";
    for (auto &i:current_namespace)
    {
        cur += "/" + i;
    }
    auto variable = std::shared_ptr<Node>(new Node(*right_value));
    variables[current_namespace][name] = variable;
    std::cout << "registering new variable '" << name 
    << "' that typed "<< variable->type->type_name <<" in " 
    << "namespace '" << cur << "'\n";
    return variable;

}

std::shared_ptr<Node> ASTGenerator::get_variable(std::string name)
{
    if (variables[current_namespace].count(name))
    {
        auto type = variables[current_namespace][name]->type;
        auto ir_generator = variables[current_namespace][name]->ir_generator;
        return std::shared_ptr<Node>(new Node(type,ir_generator));
    }
    else
    {
        std::cout << "Error: variable '" << name << "' is not declared.\n";
        exit(0);
    }
}

std::shared_ptr<Node> ASTGenerator::attach_operator(std::shared_ptr<Node> node,std::shared_ptr<Node> other,const std::string operator_type)
{
    /*try
    {
        auto special_method = node->special_methods.at(operator_type);
        std::vector<std::shared_ptr<Node>> arg;
        arg.push_back(other);   
        auto res = std::shared_ptr<Node>(new CallFunctionNode(special_method,arg));
        return res;
    }
    catch (std::out_of_range)
    {
        return node; 
    }*/
    return node;
}
